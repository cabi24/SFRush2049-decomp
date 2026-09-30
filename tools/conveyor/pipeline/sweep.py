"""Seed sweep: compile every m2c seed once and score it. No permuter.

    python3 -m tools.conveyor.pipeline.sweep submit [--flagsets a,b] [--limit N]
    python3 -m tools.conveyor.pipeline.sweep ingest
    python3 -m tools.conveyor.pipeline.sweep report

The cheapest rung of the ladder, and the one that was missing. Measured on
2026-09-24: of the first 17 byte-exact game-code matches, **12 were already
byte-identical at base** — the permuter spent four hours on each finding
nothing, because m2c's output was simply correct. A compile-and-score pass
answers that in seconds per target instead of node-days, and it is the right
first move after anything that invalidates scores (a target rebuild, a
declaration fix, a derivation change).

Scores land in `matrix_entry` with `candidate_id='m2c:<target>'` and the
target object's sha for attribution (003), so a later target change
supersedes them like any other evidence. Extracted hits stay evidence only:
promotion remains firewalled (005 FR-010).
"""
import argparse
import json
import sys
import tarfile
import tempfile
from collections import Counter
from pathlib import Path

from ..bundles.build_job import build_job_bundle
from ..client import DEFAULT_DATA, Http, load_token
from ..coordinator import db as dbmod
from ..coordinator.store import BlobStore
from . import autodecomp
from . import disasm as disasmmod

REPO = Path(__file__).resolve().parents[3]
HISTOGRAM_JSON = REPO / "build" / "m2c_histogram.json"
# Below static seeding (30), above the flywheel (60): the sweep is seconds of
# work with a high hit rate, so it should clear ahead of any long search.
SWEEP_PRIORITY = 40
CELLS_PER_JOB = 25
# Sentinel score for "compiled under gcc's syntax check, rejected by IDO".
# Large enough never to be mistaken for a near miss, recorded rather than
# dropped so the flywheel does not keep re-queueing an unbuildable seed.
COMPILE_FAILED = 999999
DEFAULT_FLAGSETS = ("-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul",
                    "-g0 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul")


def compiled_targets(conn, histogram_path=HISTOGRAM_JSON, limit=None):
    """Compiled-bucket targets with an object, smallest first."""
    histogram = json.loads(Path(histogram_path).read_text())
    if histogram.get("run", {}).get("population_complete") is not True:
        raise ValueError(f"refusing {histogram_path}: population is not complete")
    names = {name for name, result in histogram.get("targets", {}).items()
             if result.get("bucket") == "compiled"}
    rows = [row for row in conn.execute(
        "SELECT target_id,address,insn_count,population,target_o_sha,tier"
        " FROM n64_target WHERE target_o_sha IS NOT NULL ORDER BY insn_count")
        if row["target_id"] in names]
    return rows[:limit] if limit else rows


def _seed(conn, row, context_sha):
    asm = disasmmod.derive(conn, row["target_id"], context_sha=context_sha)
    return autodecomp.m2c_seed(row["target_id"], row["address"],
                               {row["target_id"]: asm})


def submit(conn, store, http, toolkit_sha, flagsets=DEFAULT_FLAGSETS,
           limit=None, histogram_path=HISTOGRAM_JSON, dry_run=False):
    """Build and submit one compile_score job per batch of cells."""
    rows = compiled_targets(conn, histogram_path, limit)
    context_sha = autodecomp._context_sha()
    cells, files, jobs = [], {}, []
    counts = Counter()

    def flush():
        if not cells:
            return
        manifest = {"job_type": "compile_score", "toolkit_sha": toolkit_sha,
                    "cells": list(cells)}
        with tempfile.TemporaryDirectory() as tmp:
            bundle, manifest_sha = build_job_bundle(
                manifest, dict(files), Path(tmp) / "job.tar.gz")
            if not dry_run:
                _, out = http.call("POST", "/api/v1/blobs",
                                   raw=bundle.read_bytes())
                jobs.append({"job_type": "compile_score",
                             "manifest_sha": manifest_sha,
                             "bundle_sha": out["sha256"],
                             "toolkit_sha": toolkit_sha,
                             "priority": SWEEP_PRIORITY, "batch": True})
        cells.clear()
        files.clear()

    for row in rows:
        try:
            source = _seed(conn, row, context_sha)
        except Exception:
            source = None
        if not source:
            counts["no_seed"] += 1
            continue
        target_o = store.get(row["target_o_sha"])
        if target_o is None:
            counts["no_target_object"] += 1
            continue
        source_name = f"{row['target_id']}.c"
        object_name = f"{row['target_id']}.o"
        files[source_name] = source.encode()
        files[object_name] = target_o.read_bytes()
        for flagset in flagsets:
            cells.append({
                "candidate_id": f"m2c:{row['target_id']}",
                "source": source_name, "flagset": flagset,
                "targets": [{"target_id": row["target_id"], "file": object_name,
                             "target_o_sha": row["target_o_sha"]}],
            })
            counts["cells"] += 1
        counts["targets"] += 1
        if len(cells) >= CELLS_PER_JOB * len(flagsets):
            flush()
    flush()
    if jobs and not dry_run:
        http.call("POST", "/api/v1/work", body=jobs)
    counts["jobs"] = len(jobs)
    return dict(counts)


def ingest(conn, store):
    """Record finished sweep cells as matrix evidence; report the hits."""
    rows = conn.execute(
        "SELECT job_id,result_sha FROM work_unit WHERE job_type='compile_score'"
        " AND state='DONE' AND result_sha IS NOT NULL AND ingested_at IS NULL"
    ).fetchall()
    counts = Counter()
    zeros = []
    for row in rows:
        path = store.get(row["result_sha"])
        if path is None:
            continue
        try:
            with tarfile.open(path) as tar:
                result = json.loads(tar.extractfile("result.json").read())
        except (tarfile.TarError, KeyError, ValueError, OSError):
            counts["unreadable"] += 1
            continue
        cells = (result.get("payload") or {}).get("cells") or []
        with dbmod.tx(conn):
            for cell in cells:
                if not str(cell.get("candidate_id", "")).startswith("m2c:"):
                    continue
                counts["cells"] += 1
                score = cell.get("score")
                target_id = cell.get("target_id") or (
                    cell.get("targets") or [{}])[0].get("target_id")
                if target_id is None:
                    counts["no_score"] += 1
                    continue
                if score is None:
                    # The seed did not compile under IDO. `matrix_entry.score`
                    # is NOT NULL, so record the sentinel: it is real evidence
                    # ("tried, cannot build with the real compiler") and it
                    # keeps the flywheel from spending node-days searching a
                    # seed the node cannot compile. The histogram's `compiled`
                    # bucket is measured with mips-linux-gnu-gcc, not IDO, and
                    # 96 of 519 seeds differ on exactly that.
                    counts["compile_failed"] += 1
                    conn.execute(
                        "INSERT OR REPLACE INTO matrix_entry (target_id,candidate_id,"
                        " flagset,toolkit_sha,score,target_o_sha)"
                        " VALUES (?,?,?,?,?,?)",
                        (target_id, cell["candidate_id"], cell.get("flagset", ""),
                         result.get("toolkit_sha", ""), COMPILE_FAILED,
                         cell.get("target_o_sha")))
                    continue
                conn.execute(
                    "INSERT OR REPLACE INTO matrix_entry (target_id,candidate_id,"
                    " flagset,toolkit_sha,score,target_o_sha)"
                    " VALUES (?,?,?,?,?,?)",
                    (target_id, cell["candidate_id"], cell.get("flagset", ""),
                     result.get("toolkit_sha", ""), score,
                     cell.get("target_o_sha")))
                if score == 0:
                    zeros.append((target_id, cell.get("flagset", "")))
            conn.execute(
                "UPDATE work_unit SET ingested_at=strftime('%Y-%m-%dT%H:%M:%fZ','now')"
                " WHERE job_id=?", (row["job_id"],))
        counts["jobs"] += 1
    counts["zero_score_cells"] = len(zeros)
    return dict(counts), sorted(set(zeros))


def report(conn):
    rows = conn.execute(
        "SELECT m.target_id, min(m.score) best, t.population FROM matrix_entry m"
        " JOIN n64_target t USING (target_id) WHERE m.candidate_id LIKE 'm2c:%'"
        " GROUP BY m.target_id ORDER BY best").fetchall()
    failed = [r for r in rows if r["best"] >= COMPILE_FAILED]
    rows = [r for r in rows if r["best"] < COMPILE_FAILED]
    zeros = [r for r in rows if r["best"] == 0]
    print(f"sweep evidence: {len(rows)} targets scored, {len(zeros)} byte-identical"
          + (f", {len(failed)} rejected by IDO" if failed else ""))
    for r in zeros:
        print(f"   MATCH  {r['target_id']} ({r['population']})")
    near = [r for r in rows if 0 < r["best"] <= 30][:15]
    if near:
        print("nearest misses:")
        for r in near:
            print(f"   {r['best']:6d}  {r['target_id']}")
    return rows


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", default=str(DEFAULT_DATA))
    parser.add_argument("--coordinator", default="http://127.0.0.1:8323")
    parser.add_argument("--token", default=None)
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("submit")
    p.add_argument("--limit", type=int, default=None)
    p.add_argument("--flagsets", default=None,
                   help="comma-separated; default is the two confirmed flagsets")
    p.add_argument("--dry-run", action="store_true")
    p = sub.add_parser("ingest")
    p = sub.add_parser("report")
    args = parser.parse_args()

    data = Path(args.data)
    conn = dbmod.connect(data / "conveyor.db")
    store = BlobStore(data / "blobs")
    if args.command == "report":
        report(conn)
        return
    http = Http(args.coordinator, load_token(args.token, data))
    if args.command == "submit":
        flagsets = tuple(f.strip() for f in args.flagsets.split(",")) \
            if args.flagsets else DEFAULT_FLAGSETS
        counts = submit(conn, store, http, http.pinned_toolkit(), flagsets,
                        args.limit, dry_run=args.dry_run)
        print("sweep submit: " + "  ".join(f"{k}={v}" for k, v in sorted(counts.items())))
        return
    counts, zeros = ingest(conn, store)
    print("sweep ingest: " + "  ".join(f"{k}={v}" for k, v in sorted(counts.items())))
    for target_id, flagset in zeros:
        print(f"   MATCH  {target_id}  @ {flagset}")


if __name__ == "__main__":
    sys.exit(main())
