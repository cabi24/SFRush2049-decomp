#!/usr/bin/env python3
"""w29n: rename GAME (extracted-population) inventory rows in place.

    python3 cloud/work/frontier/w29n/tools/db_rename.py MAP.json [--data DIR] [--apply] [--backup PATH]

`targets_rename rename` handles static rows only. Game function names are the
Conveyor inventory's target_ids (n64_target, population 'extracted'), and the
blob layout (`blob_layout derive`) takes every game function name from there,
so a game rename is: this script (DB) -> `blob_layout derive` -> `blob_tu
generate`/`symbols` (asm/us/blob) -> lock/source migration.

Only rows of kind "function" in MAP.json are used. Every keyed table moves from
the old to the new id (n64_target, function_status, matrix_entry, work_unit,
promotion_record) through temporary ids in one transaction. Target objects
(target_o_sha) are NOT rebuilt: they keep the old symbol name; game scoring
uses asm/us/blob (tools/cloud/score.py), and a farm search on a renamed row
needs `pipeline.targets --relocate-extracted` first.

Refusals (nothing written): old row missing, not 'extracted', or at another
address; new name already registered; an open work unit on an old row.
Default is a dry run; --apply writes (after a sqlite backup to --backup).
"""
import argparse
import json
import sqlite3
import sys
from pathlib import Path

KEYED = ("n64_target", "function_status", "matrix_entry", "work_unit", "promotion_record")
OPEN = ("PENDING", "LEASED", "RUNNING")   # targets_rename.OPEN_WORK_STATES


def rows(path):
    doc = json.loads(Path(path).read_text())
    return [(int(r["vram"], 16), r["old"], r["new"]) for r in doc["renames"]
            if r.get("status", "accepted") == "accepted" and r.get("kind", "function") == "function"]


def plan(conn, table):
    news = {n for _v, _o, n in table}
    olds = {o for _v, o, _n in table}
    out = []
    for vram, old, new in table:
        r = conn.execute("SELECT address, population FROM n64_target WHERE target_id=?", (old,)).fetchone()
        if r is None:
            raise SystemExit(f"refusing: {old}: no inventory row")
        if r[1] != "extracted" or r[0] != vram:
            raise SystemExit(f"refusing: {old}: not an extracted row at 0x{vram:08X} ({r[1]}, 0x{r[0]:08X})")
        if conn.execute("SELECT 1 FROM n64_target WHERE target_id=?", (new,)).fetchone() and new not in olds:
            raise SystemExit(f"refusing: {new}: already registered")
        q = ",".join("?" * len(OPEN))
        if conn.execute(f"SELECT 1 FROM work_unit WHERE target_id=? AND state IN ({q})", (old,) + OPEN).fetchone():
            raise SystemExit(f"refusing: {old}: open work units")
        counts = {t: conn.execute(f"SELECT COUNT(*) FROM {t} WHERE target_id=?", (old,)).fetchone()[0] for t in KEYED}
        out.append((vram, old, new, counts))

    return out


def apply(conn, actions):
    with conn:
        for _v, old, new, _c in actions:
            for t in KEYED:
                conn.execute(f"UPDATE {t} SET target_id=? WHERE target_id=?", ("\x00rename\x00" + new, old))
        for _v, _old, new, _c in actions:
            for t in KEYED:
                conn.execute(f"UPDATE {t} SET target_id=? WHERE target_id=?", (new, "\x00rename\x00" + new))


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("map")
    p.add_argument("--data", default=str(Path.home() / ".conveyor"))
    p.add_argument("--apply", action="store_true")
    p.add_argument("--backup")
    a = p.parse_args()
    db = Path(a.data) / "conveyor.db"
    conn = sqlite3.connect(db)
    actions = plan(conn, rows(a.map))
    for vram, old, new, counts in actions:
        print(f"rename {old:24} -> {new:22} @0x{vram:08X}  " + ", ".join(f"{t} {n}" for t, n in counts.items()))
    if not a.apply:
        print(f"dry run: nothing written ({len(actions)} renames, {db})")
        return 0
    if a.backup:
        dst = sqlite3.connect(a.backup)
        conn.backup(dst)
        dst.close()
        print(f"backup -> {a.backup}")
    apply(conn, actions)
    left = [o for _v, o, _n, _c in actions
            if conn.execute("SELECT 1 FROM n64_target WHERE target_id=?", (o,)).fetchone()]
    tmp = conn.execute("SELECT COUNT(*) FROM n64_target WHERE target_id LIKE '%rename%'").fetchone()[0]
    print(f"renamed {len(actions)} rows in {db}; old names left {left}; temporary ids left {tmp}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
