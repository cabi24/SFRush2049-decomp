#!/usr/bin/env python3
"""Lane tool (w39c): run tools.conveyor.pipeline.blob_unit with extra unit_overrides entries added in memory
(nothing is written to src/). Usage, from the repo root:
  python3 cloud/work/frontier/w39c/tools/unit_extra.py --align NAME [--align NAME ...] -- <blob_unit args>
e.g. ... --align brake_force_apply -- --tag w39c score brake_force_apply --with FILE.c --neighbours"""
import sys
sys.path.insert(0, '.')
from tools.conveyor.pipeline import blob_unit as bu
argv = sys.argv[1:]
extra = {'align': []}
while argv and argv[0] != '--':
    k = argv.pop(0).lstrip('-')
    extra.setdefault(k, []).append(argv.pop(0))
argv = argv[1:]
_orig = bu.load_overrides
def patched(path=bu.OVERRIDES):
    out = _orig(path)
    for key, names in extra.items():
        for n in names:
            out[key] = list(out[key]) + [{"name": n, "reason": "w39c lane test (in memory)"}]
    return out
bu.load_overrides = patched
sys.exit(bu.main(argv))
