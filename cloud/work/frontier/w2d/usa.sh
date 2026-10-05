#!/bin/sh
# usage: usa.sh cand.c  -- AdjustSpeed experiment in the unit
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w2d score AdjustSpeed audio_effect_process func_800960CC func_800A2670 func_800A2678 --internal func_800960CC --internal func_800A2670 --internal func_800A2678 --with "$1" 2>&1 | grep -v "^       +0x" | tail -12
grep -A12 "^  AdjustSpeed" build/blob_unit/w2d/umerge.log | grep inlining | tr '\n' ' '; echo
python3 cloud/work/frontier/w2d/objdiff.py build/blob_unit/w2d/unit.o AdjustSpeed $2 | tail -${3:-80}
