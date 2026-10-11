#!/usr/bin/env python3
"""Copy inventory target objects out of a conveyor data dir (read-only) as OUT/<target_id>.o.

    python3 collect_targets.py DATA_DIR OUT_DIR TARGET_ID [TARGET_ID...]

Used with the scratch COPY of conveyor.db after `targets_rename apply`, so check.py on
the builder scores against exactly the objects `lock add --target <newname>` would use.
Blobs missing from DATA_DIR/blobs are read from --fallback-blobs (e.g. the live store,
read-only) when given.
"""
import argparse
import shutil
import sqlite3
from pathlib import Path

ap = argparse.ArgumentParser()
ap.add_argument("data"); ap.add_argument("out"); ap.add_argument("ids", nargs="+")
ap.add_argument("--fallback-blobs", default=None)
a = ap.parse_args()
conn = sqlite3.connect(f"file:{Path(a.data) / 'conveyor.db'}?mode=ro", uri=True)
out = Path(a.out)
out.mkdir(parents=True, exist_ok=True)
for tid in a.ids:
    row = conn.execute("SELECT target_o_sha, tier, insn_count, printf('%08X', address)"
                       " FROM n64_target WHERE target_id=?", (tid,)).fetchone()
    if row is None:
        raise SystemExit(f"{tid}: no inventory row")
    src = Path(a.data) / "blobs" / row[0]
    if not src.is_file() and a.fallback_blobs:
        src = Path(a.fallback_blobs) / row[0]
    shutil.copyfile(src, out / f"{tid}.o")
    print(f"{tid:24} 0x{row[3]} {row[2]:4} insns {row[1]:11} {row[0][:12]}")
