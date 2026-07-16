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
import json
import tarfile
import time
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


def _now_sql():
    return "strftime('%Y-%m-%dT%H:%M:%fZ','now')"


def _set_status(conn, target_id, status, human_flag=None, best_score=None):
    from . import status as statusmod

    try:
        statusmod.transition(conn, target_id, status, human_flag=human_flag,
                             best_score=best_score)
    except statusmod.InvalidTransition as exc:
        # A late result for a function a human already moved on: log, don't die.
        print(f"farm: ignored {exc}")
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
    for row in rows:
        target_id = row["target_id"]
        result, artifacts = (None, {})
        if row["result_sha"]:
            result, artifacts = _read_result(store, row["result_sha"])
            if result is None:
                continue  # blob not present yet: retry next tick, don't mark
        payload = (result or {}).get("payload", {})
        target_id = target_id or payload.get("target_id")

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
                    source_sha = store.put_bytes(best_c)
                    _set_status(conn, target_id, "matched", best_score=0)
                    _submit_promotion(conn, store, http, toolkit_sha, target_id,
                                      source_sha, row["job_id"], payload)
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
                    _set_status(conn, target_id, "seeded",
                                human_flag=f"verify_failed:{outcome[:60]}")
                    rolled_back += 1

    return {"harvested": harvested, "promoted": promoted,
            "stalled": stalled, "rolled_back": rolled_back, "errored": errored}


def _submit_promotion(conn, store, http, toolkit_sha, target_id, source_sha,
                      search_job_id, search_payload):
    row = conn.execute(
        "SELECT t.target_o_sha, f.best_candidate_id, f.seed_kind FROM n64_target t"
        " JOIN function_status f USING (target_id) WHERE t.target_id=?",
        (target_id,),
    ).fetchone()
    if row is None or row["target_o_sha"] is None or store.get(row["target_o_sha"]) is None:
        # A win we cannot promote (target inventory drifted): flag it instead
        # of crashing the whole ingest run.
        _flag_only(conn, target_id, "missing_target_object")
        return
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
    http.call("POST", "/api/v1/work", body=[{
        "job_type": "verify_promote", "manifest_sha": m_sha,
        "bundle_sha": out["sha256"], "toolkit_sha": toolkit_sha,
        "target_id": target_id, "required_capability": "builder",
        "priority": 1, "batch": False, "max_attempts": 3,
    }])


def top_up(conn, store, http, toolkit_sha, max_inflight, budget_seconds):
    """Step 3: keep max_inflight searches running, best prospects first."""
    inflight = conn.execute(
        "SELECT COUNT(*) AS n FROM work_unit WHERE job_type='permuter_search'"
        " AND state IN ('PENDING','LEASED')"
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
                   priority=10)
        http.call("POST", "/api/v1/work", body=[job])
        with dbmod.tx(conn):
            _set_status(conn, p["target_id"], "in_search")
        started += 1
    return {"started": started, "inflight": inflight + started}


def run_once(conn, store, http, toolkit_sha, max_inflight, budget_seconds):
    stats = ingest(conn, store, http, toolkit_sha)
    stats.update(top_up(conn, store, http, toolkit_sha, max_inflight, budget_seconds))
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
