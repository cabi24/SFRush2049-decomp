"""Export the next single-function work items for the cloud agents.

    python3 -m tools.conveyor.pipeline.cloud_worklist [--max-score N] [--limit N] [--apply]

Picks extracted game functions that are not in the ROM, not IPA-shaped (those
are group work) and not already under cloud/work/near-miss/, ranked by the
best pipeline score any search reached. Each becomes
cloud/work/near-miss/<fn>/base.c (the whole translation unit, shim expanded,
the best source found so far) plus a row in that directory's INDEX.md, so the
cloud tools (rank_near_miss.py, gen_nearmiss_index.py) pick it up unchanged.

The pipeline score is the historical heuristic (stack offsets masked); the
cloud agents rescore strictly. Dry run unless --apply.
"""
import argparse
import gzip
import re
import sqlite3
from pathlib import Path

from ..coordinator import db as dbmod
from ..coordinator.store import BlobStore
from . import blob_splice, ipa

REPO = Path(__file__).resolve().parents[3]
NEAR_MISS = REPO / "cloud" / "work" / "near-miss"
SHIM = REPO / "tools" / "conveyor" / "seeds" / "shim" / "conveyor_shim.h"
DEFAULT_FLAGS = "-g0 -O2 -mips2 -G 0 -non_shared"


def expand_shim(text):
    """Replace the shim include with the shim's text: the cloud has no shim path."""
    body = SHIM.read_text()
    return re.sub(r'^#include\s+"conveyor_shim\.h"\s*\n', lambda _: body + "\n", text,
                  count=1, flags=re.M)


def _best_source(conn, store, target):
    """(source text, origin) from the best search result, else the seed."""
    row = conn.execute(
        "SELECT best_score, best_source_sha FROM work_unit WHERE target_id=?"
        " AND job_type='permuter_search' AND best_source_sha IS NOT NULL"
        " AND best_score IS NOT NULL ORDER BY best_score, updated_at DESC LIMIT 1",
        (target,)).fetchone()
    if row:
        blob = store.get(row["best_source_sha"])
        if blob:
            data = blob.read_bytes()
            try:
                data = gzip.decompress(data)
            except OSError:
                pass
            return data.decode(errors="replace"), "permuter"
    row = conn.execute("SELECT seed_source_sha FROM function_status WHERE target_id=?",
                       (target,)).fetchone()
    if row and row["seed_source_sha"]:
        blob = store.get(row["seed_source_sha"])
        if blob:
            return blob.read_bytes().decode(errors="replace"), "m2c sweep"
    return None, None


def candidates(conn, max_score, exclude):
    rows = conn.execute(
        "SELECT f.target_id, f.best_score, f.flagset FROM function_status f"
        " JOIN n64_target t USING(target_id) WHERE t.population='extracted'"
        " AND f.status IN ('seeded','in_search','unmatched','candidate_identified')"
        " AND f.best_score IS NOT NULL AND f.best_score BETWEEN 1 AND ?"
        " ORDER BY f.best_score, f.target_id", (max_score,)).fetchall()
    return [r for r in rows if r["target_id"] not in exclude]


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("--data", default=str(Path.home() / ".conveyor"))
    parser.add_argument("--max-score", type=int, default=1000)
    parser.add_argument("--limit", type=int, default=60)
    parser.add_argument("--apply", action="store_true")
    args = parser.parse_args(argv)
    data = Path(args.data)
    conn, store = dbmod.connect(data / "conveyor.db"), BlobStore(data / "blobs")
    conn.row_factory = sqlite3.Row

    exclude = set(blob_splice.load_lock()) | ipa.load_members()
    exclude |= {d.name for d in NEAR_MISS.iterdir() if d.is_dir()}
    picked, rows = 0, []
    for cand in candidates(conn, args.max_score, exclude):
        if picked >= args.limit:
            break
        text, origin = _best_source(conn, store, cand["target_id"])
        if not text or f"{cand['target_id']}(" not in text:
            continue
        flags = cand["flagset"] or DEFAULT_FLAGS
        rows.append((cand["target_id"], cand["best_score"], origin, flags))
        picked += 1
        if args.apply:
            d = NEAR_MISS / cand["target_id"]
            d.mkdir(parents=True, exist_ok=True)
            (d / "base.c").write_text(expand_shim(text))
    for target, score, origin, flags in rows:
        print(f"{score:6d}  {target:28} {origin:10} {flags}")
    if args.apply and rows:
        index = NEAR_MISS / "INDEX.md"
        text = index.read_text()
        marker = "\n\n## Matched (now in cloud/matches/)"
        add = "".join(f"| {t} | {s} | - | {o} | `{f}` | not yet rescored |\n"
                      for t, s, o, f in rows)
        head, sep, tail = text.partition(marker)
        index.write_text(head.rstrip("\n") + "\n" + add + (sep + tail if sep else ""))
        print(f"wrote {len(rows)} directories and index rows")
    else:
        print(f"{len(rows)} candidates (dry run)" if not args.apply else "nothing to write")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
