#!/bin/bash
# sweepanim.sh OUT FILE... : unit-score animText candidates (controlscr group as context); one line each
OUT=$1; shift
G=cloud/work/frontier/w36k/groups/controlscr
cd /home/cburnes/projects/rush2049-decomp
for f in "$@"; do
  r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag w36k score controls_animText controls_animButtonText \
    --with $G/target.c --with $G/caller.c --with $G/rows.c --with $G/predicate.c --with $G/dtc.c --with $G/d8154.c --with $G/d9058.c --with $f \
    --internal func_800D816C --internal render_replay_ui --internal func_800D8078 --internal func_800D8154 \
    --internal drone_throttle_calc --internal func_800D9058 --internal controls_animButtonText \
    --internal ct_gfx_lock --internal ct_gfx_unlock --internal ct_font_set --internal ct_CenterText --internal ct_RightText --internal ct_SetFont 2>&1)
  a=$(echo "$r" | grep -E "controls_animText:" | head -1 | sed 's/^ *//' | cut -c1-70)
  b=$(echo "$r" | grep -E "controls_animButtonText:" | head -1 | sed 's/^ *//' | cut -c1-60)
  sa=$(TAG=w36k python3 cloud/work/frontier/tools/trace/udiff.py controls_animText --summary 2>&1 | sed 's/want.*differing rows/rows/; s/unverified.*//')
  sb=$(TAG=w36k python3 cloud/work/frontier/tools/trace/udiff.py controls_animButtonText --summary 2>&1 | sed 's/want.*differing rows/rows/; s/unverified.*//')
  echo "$(basename $f) | $a | $sa | B: $sb" >> $OUT
done
echo DONE >> $OUT
