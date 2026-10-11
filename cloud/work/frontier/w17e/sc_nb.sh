#!/bin/sh
# usage: sc.sh D91A0.c D816C.c  -- unit score of the cluster with the provisional bodies
A=$(realpath $1); B=$(realpath $2); cd /home/cburnes/projects/rush2049-decomp
W=cloud/work/frontier/w17e
python3 -m tools.conveyor.pipeline.blob_unit --tag w17e --jobs 2 score func_800D91A0 func_800D816C render_replay_ui render_results_screen func_800D8078 drone_throttle_calc func_800D8154 \
  --with $A --with $B --with $W/base/rows.c --with $W/base/predicate.c --with $W/base/dtc.c --with $W/base/d8154.c --internal func_800D8154 $EXTRA \
  --internal func_800D816C --internal render_replay_ui --internal func_800D8078 --internal drone_throttle_calc $SCARGS 2>&1 | grep -E "EQUAL|FAIL|blob_unit|rror|locked bodies"
for f in func_800D91A0 func_800D816C; do
 for m in "" --ops --norm; do python3 cloud/work/frontier/tools/trace/udiff.py $f --obj build/blob_unit/w17e/unit.o --summary $m; done
done
