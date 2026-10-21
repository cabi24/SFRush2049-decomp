#!/bin/bash
# uu.sh CALLER.c [extra blob_unit args] -- unit score of controlscr cluster incl. d9058 (internal)
G=cloud/work/frontier/w36k/groups/controlscr
C=$1; shift
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w36k score func_800D91A0 func_800D9058 func_800D816C drone_throttle_calc render_replay_ui render_results_screen func_800D8078 func_800D8154 "$@" \
  --with ${TARGET:-$G/target.c} --with $C --with $G/rows.c --with $G/predicate.c --with ${DTC:-$G/dtc.c} --with ${D8154:-$G/d8154.c} --with ${D9058:-$G/d9058.c} \
  --internal func_800D816C --internal render_replay_ui --internal func_800D8078 --internal func_800D8154 \
  --internal drone_throttle_calc --internal func_800D9058 2>&1 | grep -E "EQUAL|FAIL|blob_unit score|rror" 
TAG=w36k python3 cloud/work/frontier/tools/trace/udiff.py func_800D91A0 --summary
