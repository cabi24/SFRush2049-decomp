"""Run `blob_unit` with one group's spec and sources taken from a lane directory instead of
src/blob/groups/<group>/ (the installed-group view, without editing src/). Read-only on the repo.

usage (repo root): [EXTERN=SYM,..] PYTHONPATH=$PWD python3 cloud/work/frontier/w24d/tools/unit_group.py GROUP LANE_GROUP_DIR -- \
                   --tag w24d score NAME... [--neighbours]
"""
import json
import sys
from pathlib import Path

from tools.conveyor.pipeline import blob_unit as bu

group, gdir = sys.argv[1], Path(sys.argv[2]).resolve()
rest = sys.argv[sys.argv.index("--") + 1:]
spec = json.loads((gdir / "group.json").read_text())
orig = bu.unit_files


def unit_files(lock, specs, extras=()):
    specs = dict(specs)
    specs[group] = spec
    files, problems = orig(lock, specs, extras)
    for rec in files:
        if rec["kind"] == "group" and rec["owner"] == group:
            rec["source"] = str(gdir / rec["source"].rsplit("/", 1)[1])
    return files, problems


bu.unit_files = unit_files

# EXTERN=SYM[,SYM] simulates unit_overrides.json "extern_data" entries for the group's group.c
# (file "src/blob/groups/GROUP/group.c" once installed).
import os
orig_lo = bu.load_overrides


def load_overrides(path=bu.OVERRIDES):
    doc = orig_lo(path)
    for name in filter(None, os.environ.get("EXTERN", "").split(",")):
        doc["extern_data"] = list(doc["extern_data"]) + [dict(
            name=name, file=str(gdir / "group.c"), reason="lane simulation of the integration override")]
    return doc


bu.load_overrides = load_overrides
sys.exit(bu.main(rest))
