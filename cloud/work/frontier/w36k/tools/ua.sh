#!/bin/bash
# ua.sh ANIM.c [extra] -- unit score: controlscr group (current best) + animText pair candidate
G=cloud/work/frontier/w36k/groups/controlscr
A=$1; shift
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w36k score controls_animText controls_animButtonText func_800D91A0 func_800D8154 "$@" \
  --with $G/target.c --with $G/caller.c --with $G/rows.c --with $G/predicate.c --with $G/dtc.c --with $G/d8154.c --with $G/d9058.c --with $A \
  --internal func_800D816C --internal render_replay_ui --internal func_800D8078 --internal func_800D8154 \
  --internal drone_throttle_calc --internal func_800D9058 --internal controls_animButtonText \
  --internal ct_gfx_lock --internal ct_gfx_unlock --internal ct_font_set --internal ct_CenterText --internal ct_RightText --internal ct_SetFont --internal ct_touch 2>&1 | grep -E "EQUAL|FAIL|blob_unit score|rror"
for f in controls_animText controls_animButtonText; do TAG=w36k python3 cloud/work/frontier/tools/trace/udiff.py $f --summary; done
