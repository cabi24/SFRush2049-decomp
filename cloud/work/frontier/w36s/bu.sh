#!/bin/bash
# usage: bu.sh cand.c [extra blob_unit args]  -- scores the resurrect cluster in the whole-program unit
cd /home/cburnes/projects/rush2049-decomp
C=$1; shift
python3 -m tools.conveyor.pipeline.blob_unit --tag w36s score func_800D348C stunt_combo_display camera_follow_path func_800C4F68 func_800D3B20 \
  --with $C --with cloud/work/frontier/w36s/res/camera_pair.c \
  --internal camera_follow_path --internal func_800D348C --internal func_800D3B20 "$@" 2>&1
