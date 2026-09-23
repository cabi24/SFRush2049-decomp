"""Permuter farm (US2): keep the search queue full, harvest wins, promote.

    python3 -m tools.conveyor.pipeline.farm run [--once] [--max-inflight 8]

Steady-state loop (FR-009/FR-010, no model calls anywhere):
  1. ingest finished permuter_search results
       score 0  -> status matched, submit verify_promote (builder-pinned)
       score >0 -> status seeded + human_flag=stalled (budget exhausted)
  2. ingest finished verify_promote results
       promoted    -> status verified + PromotionRecord
       rolled_back -> status seeded + human_flag, PromotionRecord audit row
  3. top up the queue: closest-to-matching candidate_identified functions get
     seeds built from their best arcade candidate and enter in_search
"""
import argparse
from dataclasses import dataclass
import http.client
import json
import socket
import tarfile
import time
import urllib.error
import uuid
from pathlib import Path

from ..bundles.build_job import build_job_bundle
from ..client import DEFAULT_DATA, Http, load_token
from ..coordinator import db as dbmod
from ..coordinator.store import BlobStore
from ..seeds import extract_candidates as extractmod
from . import seeds as seedsmod

DEFAULT_FLAGSET = "-g0 -O2 -mips2 -G 0 -non_shared"
EXTRACTED_FLAGSETS = (
    "-g0 -O2 -mips2 -G 0 -non_shared",
    "-g0 -O1 -mips2 -G 0 -non_shared",
)
REPO = Path(__file__).resolve().parents[3]
HISTOGRAM_JSON = REPO / "build" / "m2c_histogram.json"
VERIFY_PRIORITY = 1
PROMOTE_PRIORITY = 10
FLYWHEEL_PRIORITY = 60
STANDARD_SEARCH_BUDGET_SECONDS = seedsmod.DEFAULT_BUDGET["wall_seconds"]


@dataclass(frozen=True)
class FlywheelSelection:
    targets: tuple
    compiled: int
    scored: int
    in_search: int


def flywheel_selection(conn, histogram_path=HISTOGRAM_JSON):
    """Return compiling targets which have no append-only score evidence."""
    histogram = json.loads(Path(histogram_path).read_text())
    if histogram.get("run", {}).get("population_complete") is not True:
        raise ValueError(
            f"refusing {histogram_path}: run.population_complete is not true"
        )
    compiled_ids = sorted(
        target_id for target_id, result in histogram.get("targets", {}).items()
        if result.get("bucket") == "compiled"
    )
    search_rows = conn.execute(
        "SELECT DISTINCT target_id,state FROM work_unit"
        " WHERE job_type='permuter_search' AND target_id IS NOT NULL"
    ).fetchall()
    search_ids = {row["target_id"] for row in search_rows}
    active_search_ids = {row["target_id"] for row in search_rows
                         if row["state"] in ("PENDING", "LEASED")}
    completed_search_ids = {row["target_id"] for row in search_rows
                            if row["state"] == "DONE"}
    matrix_ids = {row["target_id"] for row in conn.execute(
        "SELECT DISTINCT target_id FROM matrix_entry"
    )}
    evidence_ids = search_ids | matrix_ids
    inventory = {row["target_id"]: row for row in conn.execute(
        "SELECT target_id,address,population,insn_count FROM n64_target"
    )}
    targets = tuple(
        inventory[target_id] for target_id in compiled_ids
        if target_id not in evidence_ids and target_id in inventory
    )
    compiled_set = set(compiled_ids)
    return FlywheelSelection(
        targets=targets,
        compiled=len(compiled_ids),
        scored=len(compiled_set & (matrix_ids | completed_search_ids)),
        in_search=len(compiled_set & active_search_ids),
    )


def flywheel_cycle(conn, store, http, toolkit_sha,
                   histogram_path=HISTOGRAM_JSON):
    """Submit every newly compiling extracted seed at flywheel priority."""
    from . import autodecomp

    selection = flywheel_selection(conn, histogram_path)
    asm_idx = {}
    for population in sorted({row["population"] for row in selection.targets}):
        rows = [row for row in selection.targets
                if row["population"] == population]
        asm_idx.update(autodecomp._asm_for_rows(conn, population, rows))
    started = 0
    for row in selection.targets:
        outcome = autodecomp.submit_one(
            conn, store, http, toolkit_sha, row["target_id"], row["address"],
            asm_idx,
            budget_seconds=STANDARD_SEARCH_BUDGET_SECONDS,
            priority=FLYWHEEL_PRIORITY,
        )
        started += outcome == "seeded"
    return {
        "flywheel_started": started,
        "compiled": selection.compiled,
        "scored": selection.scored,
        "in_search": selection.in_search,
    }


def _now_sql():
    return "strftime('%Y-%m-%dT%H:%M:%fZ','now')"


def _is_extracted(conn, target_id):
    row = conn.execute(
        "SELECT population FROM n64_target WHERE target_id=?", (target_id,)
    ).fetchone()
    return bool(row) and row["population"] == "extracted"


def _current_status(conn, target_id):
    row = conn.execute("SELECT status FROM function_status WHERE target_id=?",
                       (target_id,)).fetchone()
    return row["status"] if row else None


def _set_status(conn, target_id, status, human_flag=None, best_score=None):
    from . import status as statusmod

    # A search result can only ever move a target FORWARD or stall an active
    # search: a non-zero/errored result landing on a target that is already
    # matched/verified (a July search ingested in September; a sibling seed
    # that lost to a later win) is stale evidence, not a rollback — the only
    # matched -> seeded move is verify_promote's own rollback path, which
    # calls transition() directly. Found live at the 007 T009 window: one
    # ingest tick demoted 17 locked functions to seeded/stalled.
    if status == "seeded" and _current_status(conn, target_id) in ("matched", "verified"):
        print(f"farm: ignored stale {human_flag or 'seeded'} result for "
              f"{target_id} (already {_current_status(conn, target_id)})")
        return
    def apply(new_status, flag, score):
        statusmod.transition(conn, target_id, new_status, human_flag=flag,
                             best_score=score)

    try:
        try:
            apply(status, human_flag, best_score)
            return
        except statusmod.InvalidTransition as exc:
            first_failure = exc
        # A single illegal hop is often a legal PATH: a flywheel-seeded
        # target whose seeding transition was itself refused sits at
        # `unmatched`, so its eventual score-0 result read
        # `unmatched -> matched` and was dropped on the floor — the win
        # vanished (seen live 2026-09-23). Walk the chain forward instead;
        # a backward or unreachable move is still ignored.
        current = _current_status(conn, target_id)
        order = statusmod.ORDER
        if current not in order or status not in order:
            print(f"farm: ignored {first_failure}")
            return
        begin, stop = order.index(current), order.index(status)
        if stop <= begin:
            print(f"farm: ignored {first_failure}")
            return
        for step in order[begin + 1:stop + 1]:
            last = step == status
            try:
                apply(step, human_flag if last else None,
                      best_score if last else None)
            except statusmod.InvalidTransition:
                print(f"farm: ignored {first_failure}")
                return
        print(f"farm: {target_id} advanced {current} -> {status}")
    except KeyError:
        pass


def _read_result(store, result_sha):
    path = store.get(result_sha)
    if path is None:
        return None, {}
    artifacts = {}
    with tarfile.open(path) as tar:
        result = json.loads(tar.extractfile("result.json").read())
        for member in tar.getmembers():
            if member.name != "result.json":
                artifacts[member.name] = tar.extractfile(member).read()
    return result, artifacts


def _flagset_for(conn, target_id):
    # A pinned per-file flagset wins (FR-007); fall back to the O2 baseline.
    row = conn.execute(
        "SELECT f.flagset,t.population FROM function_status f"
        " LEFT JOIN n64_target t USING (target_id) WHERE f.target_id=?",
        (target_id,)
    ).fetchone()
    if row and row["flagset"]:
        return row["flagset"]
    if row and row["population"] == "extracted":
        return EXTRACTED_FLAGSETS[0]
    return DEFAULT_FLAGSET


def _mark_ingested(conn, job_id):
    conn.execute(
        f"UPDATE work_unit SET ingested_at={_now_sql()} WHERE job_id=?", (job_id,)
    )


def _flag_only(conn, target_id, human_flag):
    """Set a human-attention flag without moving status."""
    conn.execute(
        f"UPDATE function_status SET human_flag=?, updated_at={_now_sql()}"
        f" WHERE target_id=?",
        (human_flag, target_id),
    )


def ingest(conn, store, http, toolkit_sha):
    """Steps 1 and 2: pull finished search/promote jobs into pipeline state.

    Each job is processed and marked ingested in ONE transaction, so a crash
    mid-run never replays completed work nor skips unprocessed work. Results
    whose blob is missing are left un-ingested and retried next tick. FAILED
    jobs (error results past the retry cap) flag the target for attention
    instead of stranding it in in_search.
    """
    rows = conn.execute(
        "SELECT job_id, job_type, target_id, state, result_sha FROM work_unit"
        " WHERE state IN ('DONE','FAILED') AND ingested_at IS NULL"
        " AND job_type IN ('permuter_search', 'verify_promote')"
    ).fetchall()
    harvested = promoted = stalled = rolled_back = errored = 0
    extracted_wins = 0
    for row in rows:
        target_id = row["target_id"]
        result, artifacts = (None, {})
        if row["result_sha"]:
            result, artifacts = _read_result(store, row["result_sha"])
            if result is None:
                continue  # blob not present yet: retry next tick, don't mark
        payload = (result or {}).get("payload", {})
        target_id = target_id or payload.get("target_id")

        # Talk to the coordinator BEFORE opening the write transaction: the
        # coordinator shares this SQLite file, so a POST issued while we hold
        # the write lock deadlocks until its busy timeout and comes back as
        # RemoteDisconnected — which is what killed every ingest tick since
        # 005 at the first score-0 row (found at the 007 T009 window).
        source_sha, promotion, firewalled = None, None, False
        is_win = (row["state"] != "FAILED" and result and result.get("exit") == "ok"
                  and row["job_type"] == "permuter_search" and target_id
                  and payload.get("final_best_score") == 0
                  and artifacts.get("best.c"))
        if is_win:
            source_sha = store.put_bytes(artifacts["best.c"])
            # 005 FR-010: extracted-population wins are EVIDENCE ONLY — they
            # must never enter the promotion path (lock/promote enforce this
            # for hand-driven paths; the farm needs its own check, or a
            # flywheel win submits verify_promote on its own. Seen live
            # 2026-09-23: func_80095EC0 / func_800C8738).
            firewalled = _is_extracted(conn, target_id)
            if not firewalled:
                promotion = _prepare_promotion(
                    conn, store, http, toolkit_sha, target_id, source_sha,
                    row["job_id"], payload)

        with dbmod.tx(conn):
            _mark_ingested(conn, row["job_id"])
            if not target_id:
                continue

            if row["state"] == "FAILED" or (result and result.get("exit") != "ok"):
                error = (result or {}).get("error") or "job failed with no result"
                if row["job_type"] == "permuter_search":
                    _set_status(conn, target_id, "seeded",
                                human_flag=f"job_error:{error[:80]}")
                else:
                    _flag_only(conn, target_id, f"promote_error:{error[:80]}")
                errored += 1
                continue

            if row["job_type"] == "permuter_search":
                score = payload.get("final_best_score")
                best_c = artifacts.get("best.c")
                if score == 0 and best_c:
                    _set_status(conn, target_id, "matched", best_score=0)
                    if firewalled:
                        _flag_only(conn, target_id, "extracted_evidence_only")
                        extracted_wins += 1
                    elif promotion is None:
                        # Target inventory drifted: flag instead of crashing.
                        _flag_only(conn, target_id, "missing_target_object")
                    # FR-008: a win fans out to unmatched cluster siblings.
                    from . import cluster as clustermod

                    clustermod.seed_siblings(conn, store, target_id, best_c)
                    harvested += 1
                elif score is None:
                    _set_status(conn, target_id, "seeded",
                                human_flag="seed_does_not_compile")
                    stalled += 1
                else:
                    _set_status(conn, target_id, "seeded", human_flag="stalled",
                                best_score=score)
                    stalled += 1

            elif row["job_type"] == "verify_promote":
                outcome = payload.get("outcome") or "rolled_back:unknown"
                conn.execute(
                    "INSERT OR IGNORE INTO promotion_record (promotion_id,"
                    " target_id, source_sha, search_job_id, build_ok, sha1_ok,"
                    " commit_hash, doc_header_injected, outcome, created_at)"
                    f" VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, {_now_sql()})",
                    (str(uuid.uuid4()), target_id,
                     payload.get("source_sha", ""), payload.get("search_job_id"),
                     int(payload.get("build_ok", False)),
                     int(payload.get("sha1_ok", False)),
                     payload.get("commit_hash"),
                     int(payload.get("doc_header_injected", False)), outcome),
                )
                if outcome == "promoted":
                    _set_status(conn, target_id, "verified")
                    promoted += 1
                else:
                    from . import status as statusmod

                    try:  # the one legitimate matched -> seeded move (FR-010)
                        statusmod.transition(
                            conn, target_id, "seeded",
                            human_flag=f"verify_failed:{outcome[:60]}")
                    except statusmod.InvalidTransition as exc:
                        print(f"farm: ignored {exc}")
                    rolled_back += 1

    return {"harvested": harvested, "promoted": promoted,
            "stalled": stalled, "rolled_back": rolled_back, "errored": errored,
            "extracted_wins": extracted_wins}


def _prepare_promotion(conn, store, http, toolkit_sha, target_id, source_sha,
                       search_job_id, search_payload):
    """Build and submit the verify_promote job (HTTP only — call this OUTSIDE
    any write transaction).  Returns the coordinator's response, or None when
    the target object is missing (the caller flags the row)."""
    row = conn.execute(
        "SELECT t.target_o_sha, f.best_candidate_id, f.seed_kind FROM n64_target t"
        " JOIN function_status f USING (target_id) WHERE t.target_id=?",
        (target_id,),
    ).fetchone()
    if row is None or row["target_o_sha"] is None or store.get(row["target_o_sha"]) is None:
        return None
    if row["seed_kind"] == "sibling":
        provenance = f"cluster sibling seed (see cluster of {target_id})"
    else:
        provenance = row["best_candidate_id"] or "manual seed"
    manifest = {
        "job_type": "verify_promote",
        "toolkit_sha": toolkit_sha,
        "target_id": target_id,
        "source_sha": source_sha,
        "search_job_id": search_job_id,
        "candidate_id": provenance,
        "compile_flags": _flagset_for(conn, target_id),
        "score_history": f"base {search_payload.get('base_score')} -> 0",
    }
    import tempfile

    with tempfile.TemporaryDirectory() as tmp:
        bundle, m_sha = build_job_bundle(
            manifest,
            {"promoted.c": store.get(source_sha).read_bytes(),
             "target.o": store.get(row["target_o_sha"]).read_bytes()},
            Path(tmp) / "promote.tar.gz",
        )
        _, out = http.call("POST", "/api/v1/blobs", raw=bundle.read_bytes())
    _, submitted = http.call("POST", "/api/v1/work", body=[{
        "job_type": "verify_promote", "manifest_sha": m_sha,
        "bundle_sha": out["sha256"], "toolkit_sha": toolkit_sha,
        "target_id": target_id, "required_capability": "builder",
        "priority": VERIFY_PRIORITY, "batch": False, "max_attempts": 3,
    }])
    return submitted or True


def top_up(conn, store, http, toolkit_sha, max_inflight, budget_seconds):
    """Step 3: keep max_inflight searches running, best prospects first."""
    # Flywheel (priority 60) jobs are background fill: they must never count
    # against the static search budget, or a full flywheel queue would starve
    # static top-up (SC-005 in reverse; seen live with 94 queued seeds).
    inflight = conn.execute(
        "SELECT COUNT(*) AS n FROM work_unit WHERE job_type='permuter_search'"
        " AND state IN ('PENDING','LEASED') AND priority < ?",
        (FLYWHEEL_PRIORITY,)
    ).fetchone()["n"]
    to_start = max(0, max_inflight - inflight)
    if to_start == 0:
        return {"started": 0, "inflight": inflight}

    prospects = conn.execute(
        "SELECT f.target_id, f.best_candidate_id, f.seed_source_sha,"
        " f.best_score, t.insn_count"
        " FROM function_status f JOIN n64_target t USING (target_id)"
        " WHERE f.status='candidate_identified'"
        " AND (f.best_candidate_id IS NOT NULL OR f.seed_source_sha IS NOT NULL)"
        " ORDER BY CAST(f.best_score AS REAL) / MAX(t.insn_count, 1), f.target_id"
        " LIMIT ?",
        (to_start,),
    ).fetchall()
    started = 0
    for p in prospects:
        if p["seed_source_sha"]:
            # Sibling/manual seed (FR-008): source is a stored blob.
            blob = store.get(p["seed_source_sha"])
            if blob is None:
                with dbmod.tx(conn):
                    _flag_only(conn, p["target_id"], "seed_blob_missing")
                continue
            source = seedsmod.seed_source(blob.read_text())
        else:
            try:
                body = extractmod.get_body(p["best_candidate_id"])
            except (KeyError, OSError):
                with dbmod.tx(conn):
                    _flag_only(conn, p["target_id"], "candidate_body_missing")
                continue
            source = seedsmod.seed_source(body)
        bundle, m_sha, job = seedsmod.build_search_bundle(
            conn, store, p["target_id"], source, _flagset_for(conn, p["target_id"]),
            toolkit_sha, budget={"wall_seconds": budget_seconds, "iterations": None},
        )
        _, out = http.call("POST", "/api/v1/blobs", raw=bundle.read_bytes())
        job.update(bundle_sha=out["sha256"], target_id=p["target_id"],
                   priority=PROMOTE_PRIORITY)
        http.call("POST", "/api/v1/work", body=[job])
        with dbmod.tx(conn):
            _set_status(conn, p["target_id"], "in_search")
        started += 1
    return {"started": started, "inflight": inflight + started}


# Coordinator-path failures the daemon must survive: a dropped connection,
# a refused/reset socket, a read timeout. Anything else is a real bug and
# still propagates.
TRANSIENT_ERRORS = (
    urllib.error.URLError, http.client.HTTPException, ConnectionError,
    socket.timeout, TimeoutError,
)
TRANSIENT_RETRY_DELAY = 5


def with_transient_retry(step, *args, retries=1, delay=TRANSIENT_RETRY_DELAY,
                         sleep=time.sleep, log=print, **kwargs):
    """Run one daemon step; on a transient coordinator error retry it once
    after `delay`, then log and skip (return None). Never raises transient
    errors, so a flaky coordinator path can't kill the loop (006 T012)."""
    attempt = 0
    while True:
        try:
            return step(*args, **kwargs)
        except TRANSIENT_ERRORS as exc:
            attempt += 1
            name = getattr(step, "__name__", repr(step))
            if attempt > retries:
                log(f"farm: {name} skipped this cycle after {attempt} attempts: "
                    f"{type(exc).__name__}: {exc}")
                return None
            log(f"farm: {name} transient {type(exc).__name__}: {exc}; "
                f"retrying in {delay}s")
            sleep(delay)


def run_once(conn, store, http, toolkit_sha, max_inflight, budget_seconds):
    stats = {}
    for step, args in (
            (ingest, (conn, store, http, toolkit_sha)),
            (flywheel_cycle, (conn, store, http, toolkit_sha)),
            (top_up, (conn, store, http, toolkit_sha, max_inflight,
                      budget_seconds))):
        out = with_transient_retry(step, *args)
        if out:
            stats.update(out)
    return stats


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", default=str(DEFAULT_DATA))
    parser.add_argument("--coordinator", default="http://127.0.0.1:8323")
    parser.add_argument("--token", default=None)
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("run")
    p.add_argument("--once", action="store_true")
    p.add_argument("--max-inflight", type=int, default=8)
    p.add_argument("--budget-seconds", type=int, default=4 * 3600)
    p.add_argument("--interval", type=int, default=60)
    args = parser.parse_args()

    data = Path(args.data)
    conn = dbmod.connect(data / "conveyor.db")
    store = BlobStore(data / "blobs")
    http = Http(args.coordinator, load_token(args.token, data))
    toolkit_sha = http.pinned_toolkit()

    while True:
        stats = run_once(conn, store, http, toolkit_sha,
                         args.max_inflight, args.budget_seconds)
        print(f"farm: {stats}")
        if args.once:
            break
        time.sleep(args.interval)


if __name__ == "__main__":
    main()
