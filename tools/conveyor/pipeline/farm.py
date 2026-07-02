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
import sys
import tarfile
import time
import urllib.request
import uuid
from pathlib import Path

from ..bundles.build_job import build_job_bundle
from ..coordinator import db as dbmod
from ..coordinator.store import BlobStore
from ..seeds import extract_candidates as extractmod
from . import seeds as seedsmod

DEFAULT_DATA = Path("~/.conveyor").expanduser()
DEFAULT_FLAGSET = "-g0 -O2 -mips2 -G 0 -non_shared"
LEDGER_KEY = "farm_ingested_jobs"


class Http:
    def __init__(self, base, token):
        self.base, self.token = base.rstrip("/"), token

    def call(self, method, path, body=None, raw=None):
        headers = {"Authorization": f"Bearer {self.token}"}
        data = None
        if body is not None:
            data = json.dumps(body).encode()
        elif raw is not None:
            data = raw
            headers["Content-Type"] = "application/gzip"
        req = urllib.request.Request(self.base + path, data=data, method=method,
                                     headers=headers)
        with urllib.request.urlopen(req, timeout=300) as resp:
            payload = resp.read()
            return resp.status, json.loads(payload) if payload else None


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
        "SELECT flagset FROM function_status WHERE target_id=?", (target_id,)
    ).fetchone()
    return (row["flagset"] if row and row["flagset"] else DEFAULT_FLAGSET)


def ingest(conn, store, http, toolkit_sha):
    """Steps 1 and 2: pull finished search/promote jobs into pipeline state."""
    seen = set(json.loads(dbmod.get_meta(conn, LEDGER_KEY) or "[]"))
    rows = conn.execute(
        "SELECT job_id, job_type, target_id, result_sha FROM work_unit"
        " WHERE state='DONE' AND result_sha IS NOT NULL"
        " AND job_type IN ('permuter_search', 'verify_promote')"
    ).fetchall()
    harvested = promoted = stalled = rolled_back = 0
    for row in rows:
        if row["job_id"] in seen:
            continue
        result, artifacts = _read_result(store, row["result_sha"])
        seen.add(row["job_id"])
        if result is None or result["exit"] != "ok":
            continue
        payload = result["payload"]
        target_id = row["target_id"] or payload.get("target_id")
        if not target_id:
            continue

        if row["job_type"] == "permuter_search":
            score = payload.get("final_best_score")
            best_c = artifacts.get("best.c")
            with dbmod.tx(conn):
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
            with dbmod.tx(conn):
                conn.execute(
                    "INSERT INTO promotion_record (promotion_id, target_id,"
                    " source_sha, search_job_id, build_ok, sha1_ok, commit_hash,"
                    f" doc_header_injected, outcome, created_at)"
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

    dbmod.set_meta(conn, LEDGER_KEY, json.dumps(sorted(seen)))
    return {"harvested": harvested, "promoted": promoted,
            "stalled": stalled, "rolled_back": rolled_back}


def _submit_promotion(conn, store, http, toolkit_sha, target_id, source_sha,
                      search_job_id, search_payload):
    row = conn.execute(
        "SELECT t.target_o_sha, f.best_candidate_id FROM n64_target t"
        " JOIN function_status f USING (target_id) WHERE t.target_id=?",
        (target_id,),
    ).fetchone()
    manifest = {
        "job_type": "verify_promote",
        "toolkit_sha": toolkit_sha,
        "target_id": target_id,
        "source_sha": source_sha,
        "search_job_id": search_job_id,
        "candidate_id": row["best_candidate_id"] if row else None,
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
        "SELECT f.target_id, f.best_candidate_id, f.best_score, t.insn_count"
        " FROM function_status f JOIN n64_target t USING (target_id)"
        " WHERE f.status='candidate_identified' AND f.best_candidate_id IS NOT NULL"
        " ORDER BY CAST(f.best_score AS REAL) / MAX(t.insn_count, 1), f.target_id"
        " LIMIT ?",
        (to_start,),
    ).fetchall()
    started = 0
    for p in prospects:
        if p["best_candidate_id"].startswith("matched:"):
            # Sibling seed (FR-008): source is the matched member's blob.
            override = conn.execute(
                "SELECT override FROM function_status WHERE target_id=?",
                (p["target_id"],),
            ).fetchone()["override"]
            sha = json.loads(override or "{}").get("sibling_seed_sha")
            blob = store.get(sha) if sha else None
            if blob is None:
                continue
            source = seedsmod.seed_source(blob.read_text())
        else:
            try:
                body = extractmod.get_body(p["best_candidate_id"])
            except (KeyError, OSError):
                with dbmod.tx(conn):
                    _set_status(conn, p["target_id"], "candidate_identified",
                                human_flag="candidate_body_missing")
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
    token = args.token or (data / "token").read_text().strip()
    http = Http(args.coordinator, token)
    status, meta = http.call("GET", "/api/v1/meta/toolkit_sha")
    if status != 200:
        sys.exit("no toolkit pinned — publish-toolkit first")
    toolkit_sha = meta["value"]

    while True:
        stats = run_once(conn, store, http, toolkit_sha,
                         args.max_inflight, args.budget_seconds)
        print(f"farm: {stats}")
        if args.once:
            break
        time.sleep(args.interval)


if __name__ == "__main__":
    main()
