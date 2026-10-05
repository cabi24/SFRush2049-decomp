#!/usr/bin/env python3
"""Tiny whole-program builds for mechanism probes.

    python3 wp/mini.py NAME --files a.c b.c ... --keep f g ... --members f g [--order]

Files are taken as given (paths relative to the copy root, or absolute); link
order = argument order.  Prints member status and emitted order.
"""
import argparse
import json
import shutil
import sys
from pathlib import Path
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import wp  # noqa: E402


def build(name, files, keep, members, quiet=False):
    work = wp.RUNS / "mini" / name
    if work.exists():
        shutil.rmtree(work)
    work.mkdir(parents=True)
    units = []
    for k, f in enumerate(files):
        p = Path(f)
        p = p if p.is_absolute() else wp.ROOT / p
        fid = f"m{k:03d}_{p.stem}.c"
        (work / fid).write_text(wp.preprocessed(p) if p.suffix == ".c" else p.read_text())
        _, rc, msg = wp.cc_j(work, fid, wp.O3)
        if rc:
            raise SystemExit(f"cc -j {fid}: {msg}")
        units.append(fid[:-2] + ".u")
    (work / "keep.txt").write_text("".join(k + "\n" for k in keep))
    times, err = wp.stages(work, units, work / "o.o", work / "log")
    if err:
        raise SystemExit(err)
    res = wp.score_members(work / "o.o", members)
    syms = wp.score.symbols(work / "o.o")
    order = [n for n, _ in sorted(syms.items(), key=lambda kv: kv[1])]
    if not quiet:
        for m in members:
            r = res[m]
            print(f"  {m}: {r['status']} target {r.get('total')}w got {r.get('words')}w differ {r.get('differing')}")
        print("  emitted order:", " ".join(order))
    return res, order


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("name")
    ap.add_argument("--files", nargs="+")
    ap.add_argument("--keep", nargs="*", default=[])
    ap.add_argument("--members", nargs="+")
    a = ap.parse_args()
    build(a.name, a.files, a.keep, a.members)
