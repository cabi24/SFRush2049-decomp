#!/bin/sh
# w24a conversion of segment 0x26F60 (coordinator, Pi, repo root, clean tree under the affected paths).
# Equivalent to `git apply cloud/work/frontier/w24a/conversion.diff` (which was produced by exactly these steps on master 3bec9ce3).
set -e
python3 -m tools.conveyor.pipeline.layout convert 0x26f60      # splat.us.yaml asm -> c, rom/lib_26f60; make extract; TU + nonmatchings
python3 - <<'PY'
import json, collections
p = 'rom_compilers.json'
d = json.load(open(p), object_pairs_hook=collections.OrderedDict)
d['segments']['0x26f60'] = 'ido7.1'
open(p, 'w').write(json.dumps(d, indent=2) + '\n')
PY
python3 -c "from tools.conveyor.pipeline import layout; layout.write_opt_overrides()"   # adds lib_26f60.o: CC := $(IDO71_CC)
git status --short -- splat.us.yaml rom_compilers.json src/rom asm/us
