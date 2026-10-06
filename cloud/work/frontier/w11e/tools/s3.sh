#!/bin/sh
# usage: s3.sh CAND.c [extra args] -- score the engine trio in the unit (tag w11e) + aligned rows
c=$(realpath "$1"); shift
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag ${TAG:-w11e} score engine_torque_calc transmission_ratio_get engine_sound_update --internal engine_torque_calc --internal func_800AB7D0 "$@" --with "$c" 2>&1 | grep -E "EQUAL|FAIL|rror" | head -6
for f in engine_torque_calc transmission_ratio_get engine_sound_update; do echo "$f: $(python3 cloud/work/frontier/w11e/tools/udiff.py $f --obj build/blob_unit/${TAG:-w11e}/unit.o | tail -1)"; done
