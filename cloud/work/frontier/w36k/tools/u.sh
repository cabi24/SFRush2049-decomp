#!/bin/bash
# Usage: u.sh GROUPDIR [extra names...]  -- whole-program unit score of the controlscr cluster
# Run from repo root. CALLER env overrides caller.c path.
G=${1:-cloud/work/frontier/w36k/groups/controlscr}; shift
C=${CALLER:-$G/caller.c}
T=${TARGET:-$G/target.c}
D=${DTC:-$G/dtc.c}
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w36k score func_800D91A0 func_800D816C drone_throttle_calc render_replay_ui render_results_screen func_800D8078 func_800D8154 "$@" \
  --with $T --with $C --with $G/rows.c --with $G/predicate.c --with $D --with $G/d8154.c \
  --internal func_800D816C --internal render_replay_ui --internal func_800D8078 --internal func_800D8154 \
  --internal drone_throttle_calc 2>&1 | grep -E "EQUAL|FAIL|blob_unit score|error|Error" 
TAG=w36k python3 cloud/work/frontier/tools/trace/udiff.py func_800D91A0 --summary
TAG=w36k python3 cloud/work/frontier/tools/trace/udiff.py func_800D91A0 --norm --summary
