#!/usr/bin/env python3
"""w23m: recompile every locked game body whose object is not bound to its
lock pin (provenance mismatch or missing object), on the builder in this
lane's scratch. Singles per flagset (blob_splice.compile_on_builder), groups
(blob_group.compile_group). Lock entries are not touched.

    python3 cloud/work/frontier/w23m/rebuild_stale.py [--skip-group NAME ...]
"""
import argparse
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(REPO))
from tools.conveyor.pipeline import blob_group, blob_splice  # noqa: E402

WORK = "rush2049/scratch/frontier/w23m"


def stale(lock):
    singles, groups = {}, set()
    for t, e in sorted(lock.items()):
        if e.get("group"):
            obj = blob_group.object_path(e["group"], blob_splice.OBJ_DIR / "groups")
        else:
            obj = blob_splice.OBJ_DIR / f"{t}.o"
        if not obj.is_file() or blob_splice.provenance_problem(obj, e, required=True):
            if e.get("group"):
                groups.add(e["group"])
            else:
                singles[t] = e
    return singles, groups


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--skip-group", action="append", default=[])
    p.add_argument("--dry-run", action="store_true")
    a = p.parse_args()
    lock = blob_splice.load_lock()
    singles, groups = stale(lock)
    groups -= set(a.skip_group)
    print(f"stale: {len(singles)} singles, {len(groups)} groups")
    if a.dry_run:
        print(sorted(singles), sorted(groups))
        return 0
    by_flags = {}
    for t, e in singles.items():
        by_flags.setdefault(e["flagset"], {})[t] = REPO / e["source"]
    for flags, sources in sorted(by_flags.items()):
        built, failed = blob_splice.compile_on_builder(sources, flags, workdir=f"{WORK}/splice")
        if failed:
            raise SystemExit(f"compile failed: {failed}")
        print(f"compiled {len(built)} singles ({flags})")
    for g in sorted(groups):
        blob_group.compile_group(blob_group.load(g), workdir=f"{WORK}/group")
        print(f"compiled group {g}")
    s2, g2 = stale(blob_splice.load_lock())
    print(f"still stale: {len(s2)} singles {sorted(s2)[:10]}, groups {sorted(g2)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
