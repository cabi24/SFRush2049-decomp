#!/bin/sh
# us3.sh FILE.c... : unit-score the three group members with swd.c, func_80090308 internal
L=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w26c/f308
for f in "$@"; do
  p=$(realpath $f)
  echo "== $f"
  (cd /home/cburnes/projects/rush2049-decomp && python3 -m tools.conveyor.pipeline.blob_unit --tag w26c --jobs 2 score func_80090308 entity_physics_update save_write_data --with $p --with ${SWD:-$L/swd_best.c} --internal func_80090308 --neighbours 2>&1 | grep -E "EQUAL|FAIL|differ in|rror" | cut -c1-150)
done
