"""Match-matrix orchestration (US1): candidates × flagsets × targets.

    python3 -m tools.conveyor.pipeline.matrix extract   # inventories (local)
    python3 -m tools.conveyor.pipeline.matrix submit    # batch jobs -> pool
    python3 -m tools.conveyor.pipeline.matrix ingest    # results -> matrix_entry
    python3 -m tools.conveyor.pipeline.matrix report    # coverage + rankings

Pruning: scoring every pair is ~5.8M cells; a size window (candidate body
length vs target instruction count) cuts this ~10x with negligible recall
loss — a 10-instruction target never matches a 500-line candidate. Cells are
grouped so each job compiles a candidate once and scores it against every
size-compatible target (bundled targets are shared across the job's
candidates). Every batch is a pure function of its manifest: identical
re-submissions are answered from the result cache (FR-006).
"""
import argparse
import hashlib
import json
import sys
import tarfile
import tempfile
import urllib.request
from pathlib import Path

from ..bundles import manifest as manifestmod
from ..bundles.build_job import build_job_bundle
from ..coordinator import db as dbmod
from ..coordinator.store import BlobStore
from ..seeds import extract_candidates as extractmod
from . import targets as targetsmod

DEFAULT_DATA = Path("~/.conveyor").expanduser()

# Confirmed baseline flag sets (docs/COMPILER_SETTINGS.md).
FLAGSETS = (
    "-g0 -O1 -mips2 -G 0 -non_shared",
    "-g0 -O2 -mips2 -G 0 -non_shared",
)

CHARS_PER_INSN = 11        # crude proxy: candidate body chars -> insn estimate
SIZE_WINDOW = (0.25, 4.0)  # target insns must be within this factor of proxy
CANDIDATES_PER_JOB = 20
NO_ANCESTRY_FACTOR = 3.0   # best score > factor*insn_count => no arcade ancestry
HIGH_CONF_FACTOR = 0.5     # best score <= factor*insn_count ...
HIGH_CONF_MARGIN = 1.25    # ...and runner-up >= margin*best => high confidence


def _conn_store(data):
    data = Path(data)
    return dbmod.connect(data / "conveyor.db"), BlobStore(data / "blobs")


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


# --- extract ----------------------------------------------------------------

def cmd_extract(args):
    conn, store = _conn_store(args.data)
    t = targetsmod.populate(conn, store)
    c = extractmod.populate(conn)
    print(f"targets:    {t['built']} built / {t['total']} inventory "
          f"({t['skipped']} skipped)")
    print(f"candidates: {c['candidates']} functions from {c['files']} files")


# --- submit -----------------------------------------------------------------

def _size_estimate(body):
    code = "\n".join(
        line for line in body.splitlines() if line.strip() and not
        line.strip().startswith(("/*", "*", "//"))
    )
    return max(len(code) // CHARS_PER_INSN, 8)


def _existing_cells(conn, toolkit_sha):
    rows = conn.execute(
        "SELECT target_id, candidate_id, flagset FROM matrix_entry"
        " WHERE toolkit_sha = ?", (toolkit_sha,),
    )
    return {(r["target_id"], r["candidate_id"], r["flagset"]) for r in rows}


def cmd_submit(args):
    conn, store = _conn_store(args.data)
    http = Http(args.coordinator, _token(args))
    status, meta = http.call("GET", "/api/v1/meta/toolkit_sha")
    if status != 200:
        sys.exit("no toolkit pinned — publish-toolkit first")
    toolkit_sha = meta["value"]

    targets = conn.execute(
        "SELECT target_id, insn_count, target_o_sha FROM n64_target"
        " WHERE target_o_sha IS NOT NULL ORDER BY insn_count"
    ).fetchall()
    candidates = conn.execute(
        "SELECT candidate_id FROM arcade_candidate ORDER BY candidate_id"
    ).fetchall()
    done = _existing_cells(conn, toolkit_sha)
    print(f"{len(targets)} targets × {len(candidates)} candidates × "
          f"{len(args.flagsets)} flagsets; {len(done)} cells already scored")

    jobs, cells_planned = [], 0
    batch = []

    def flush_batch(flagset):
        nonlocal cells_planned
        if not batch:
            return
        cells, files = [], {}
        for cand_id, body, matched_targets in batch:
            source_name = hashlib.sha256(cand_id.encode()).hexdigest()[:16] + ".c"
            files[source_name] = (
                '#include "conveyor_shim.h"\n\n' + body + "\n"
            ).encode()
            cell_targets = []
            for t in matched_targets:
                o_name = t["target_id"] + ".o"
                if o_name not in files:
                    files[o_name] = store.get(t["target_o_sha"]).read_bytes()
                cell_targets.append({"target_id": t["target_id"], "file": o_name})
                cells_planned += 1
            cells.append({
                "candidate_id": cand_id, "source": source_name,
                "flagset": flagset, "targets": cell_targets,
            })
        m = {"job_type": "compile_score", "toolkit_sha": toolkit_sha, "cells": cells}
        with tempfile.TemporaryDirectory() as tmp:
            bundle, m_sha = build_job_bundle(m, files, Path(tmp) / "job.tar.gz")
            _, out = http.call("POST", "/api/v1/blobs", raw=bundle.read_bytes())
        jobs.append({
            "job_type": "compile_score", "manifest_sha": m_sha,
            "bundle_sha": out["sha256"], "toolkit_sha": toolkit_sha,
            "priority": 100,
        })
        batch.clear()

    for flagset in args.flagsets:
        for (cand_id,) in candidates:
            try:
                body = extractmod.get_body(cand_id)
            except (KeyError, OSError):
                continue
            proxy = _size_estimate(body)
            lo, hi = proxy * SIZE_WINDOW[0], proxy * SIZE_WINDOW[1]
            matched = [
                t for t in targets
                if t["insn_count"] and lo <= t["insn_count"] <= hi
                and (t["target_id"], cand_id, flagset) not in done
            ]
            if not matched:
                continue
            batch.append((cand_id, body, matched))
            if len(batch) >= CANDIDATES_PER_JOB:
                flush_batch(flagset)
        flush_batch(flagset)

    if args.dry_run:
        print(f"dry run: {len(jobs)} jobs, {cells_planned} cells")
        return
    submitted = cached = 0
    for i in range(0, len(jobs), 100):
        _, out = http.call("POST", "/api/v1/work", body=jobs[i : i + 100])
        for entry in out:
            if "cached_result" in entry:
                cached += 1
            else:
                submitted += 1
    print(f"submitted {submitted} jobs ({cells_planned} cells), {cached} cache hits")


# --- ingest -----------------------------------------------------------------

def cmd_ingest(args):
    conn, store = _conn_store(args.data)
    rows = conn.execute(
        "SELECT job_id, result_sha, toolkit_sha FROM work_unit"
        " WHERE job_type='compile_score' AND state='DONE' AND result_sha IS NOT NULL"
    ).fetchall()
    ingested_key = "matrix_ingested_jobs"
    seen = set(json.loads(dbmod.get_meta(conn, ingested_key) or "[]"))
    new_cells, compile_fail = 0, {}
    for row in rows:
        if row["job_id"] in seen:
            continue
        path = store.get(row["result_sha"])
        if path is None:
            continue
        with tarfile.open(path) as tar:
            result = json.loads(tar.extractfile("result.json").read())
        if result["exit"] != "ok":
            seen.add(row["job_id"])
            continue
        with dbmod.tx(conn):
            for cell in result["payload"]["cells"]:
                key = (cell["candidate_id"], cell["flagset"])
                if cell["compile"] != "ok":
                    compile_fail[key] = cell["compile"]
                    continue
                compile_fail.setdefault(key, "ok")
                conn.execute(
                    "INSERT OR IGNORE INTO matrix_entry"
                    " (target_id, candidate_id, flagset, toolkit_sha, score)"
                    " VALUES (?, ?, ?, ?, ?)",
                    (cell["target_id"], cell["candidate_id"], cell["flagset"],
                     row["toolkit_sha"], cell["score"]),
                )
                new_cells += 1
        seen.add(row["job_id"])
    with dbmod.tx(conn):
        for (cand_id, flagset), status in compile_fail.items():
            row = conn.execute(
                "SELECT compile_status FROM arcade_candidate WHERE candidate_id=?",
                (cand_id,),
            ).fetchone()
            if row is None:
                continue
            cs = json.loads(row["compile_status"])
            cs[flagset] = status
            conn.execute(
                "UPDATE arcade_candidate SET compile_status=? WHERE candidate_id=?",
                (json.dumps(cs, sort_keys=True), cand_id),
            )
    dbmod.set_meta(conn, ingested_key, json.dumps(sorted(seen)))
    print(f"ingested {new_cells} new cells from {len(rows)} done jobs")
    update_rankings(conn)


# --- rankings / no-ancestry (T024) -------------------------------------------

def rankings_for(conn, target_id, limit=10):
    """Ranked candidates for one target — stable (score, candidate_id) order."""
    return conn.execute(
        "SELECT candidate_id, flagset, MIN(score) AS score FROM matrix_entry"
        " WHERE target_id = ? GROUP BY candidate_id"
        " ORDER BY score, candidate_id LIMIT ?",
        (target_id, limit),
    ).fetchall()


def update_rankings(conn):
    """Refresh function_status from matrix state (unmatched->candidate_identified,
    no-ancestry flagging, SC-001 high-confidence marking)."""
    targets = conn.execute(
        "SELECT t.target_id, t.insn_count, f.status FROM n64_target t"
        " JOIN function_status f USING (target_id)"
    ).fetchall()
    promoted = flagged = high_conf = 0
    with dbmod.tx(conn):
        for t in targets:
            top = conn.execute(
                "SELECT candidate_id, MIN(score) AS score FROM matrix_entry"
                " WHERE target_id=? GROUP BY candidate_id"
                " ORDER BY score, candidate_id LIMIT 2",
                (t["target_id"],),
            ).fetchall()
            if not top or top[0]["score"] is None:
                continue
            best = top[0]
            insns = t["insn_count"] or 1
            if best["score"] > NO_ANCESTRY_FACTOR * insns:
                if t["status"] == "unmatched":
                    conn.execute(
                        "UPDATE function_status SET human_flag='no_ancestry',"
                        " best_score=?, updated_at=strftime('%Y-%m-%dT%H:%M:%fZ','now')"
                        " WHERE target_id=? AND override IS NULL",
                        (best["score"], t["target_id"]),
                    )
                    flagged += 1
                continue
            is_high_conf = best["score"] <= HIGH_CONF_FACTOR * insns and (
                len(top) < 2 or top[1]["score"] >= HIGH_CONF_MARGIN * max(best["score"], 1)
            )
            if is_high_conf:
                high_conf += 1
            if t["status"] == "unmatched":
                conn.execute(
                    "UPDATE function_status SET status='candidate_identified',"
                    " best_score=?, best_candidate_id=?, human_flag=NULL,"
                    " updated_at=strftime('%Y-%m-%dT%H:%M:%fZ','now')"
                    " WHERE target_id=? AND override IS NULL",
                    (best["score"], best["candidate_id"], t["target_id"]),
                )
                promoted += 1
    print(f"rankings: {promoted} targets -> candidate_identified, "
          f"{flagged} flagged no_ancestry, {high_conf} high-confidence (SC-001)")


# --- report (T025) -----------------------------------------------------------

def cmd_report(args):
    conn, _ = _conn_store(args.data)
    n_targets = conn.execute("SELECT COUNT(*) AS n FROM n64_target").fetchone()["n"]
    n_cands = conn.execute("SELECT COUNT(*) AS n FROM arcade_candidate").fetchone()["n"]
    n_cells = conn.execute("SELECT COUNT(*) AS n FROM matrix_entry").fetchone()["n"]
    print(f"matrix: {n_cells} cells scored ({n_targets} targets, {n_cands} candidates)")

    rows = conn.execute("SELECT compile_status FROM arcade_candidate").fetchall()
    ok = fail = untried = 0
    for r in rows:
        cs = json.loads(r["compile_status"])
        if not cs:
            untried += 1
        elif any(v == "ok" for v in cs.values()):
            ok += 1
        else:
            fail += 1
    if ok + fail:
        print(f"candidate compile coverage (FR-002): {ok} ok, {fail} fail, "
              f"{untried} untried  ({100 * ok // max(ok + fail, 1)}% of tried)")

    rows = conn.execute(
        "SELECT status, COUNT(*) AS n FROM function_status GROUP BY status"
    ).fetchall()
    print("function status: " + "  ".join(f"{r['status']}={r['n']}" for r in rows))
    if args.target:
        print(f"\ntop candidates for {args.target}:")
        for r in rankings_for(conn, args.target):
            print(f"  {r['score']:>7}  {r['candidate_id']}")


def _token(args):
    if args.token:
        return args.token
    token_file = DEFAULT_DATA / "token"
    if token_file.is_file():
        return token_file.read_text().strip()
    sys.exit("no --token and ~/.conveyor/token not found")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", default=str(DEFAULT_DATA))
    parser.add_argument("--coordinator", default="http://127.0.0.1:8323")
    parser.add_argument("--token", default=None)
    sub = parser.add_subparsers(dest="command", required=True)

    p = sub.add_parser("extract")
    p.set_defaults(func=cmd_extract)

    p = sub.add_parser("submit")
    p.add_argument("--flagsets", nargs="*", default=list(FLAGSETS))
    p.add_argument("--dry-run", action="store_true")
    p.set_defaults(func=cmd_submit)

    p = sub.add_parser("ingest")
    p.set_defaults(func=cmd_ingest)

    p = sub.add_parser("report")
    p.add_argument("--target", default=None)
    p.set_defaults(func=cmd_report)

    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
