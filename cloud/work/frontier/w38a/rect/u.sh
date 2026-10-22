#!/bin/sh
c=$(readlink -f $1); shift
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w38a score drone_collision_avoid --with $c --internal drone_collision_avoid --internal du_gfx_lock --internal du_gfx_unlock --internal du_font_set --internal du_SetFont --internal du_HAL_SetFont --internal du_validOption "$@" 2>&1 | grep -E "EQUAL|FAIL" | cut -c1-150
TAG=w38a python3 cloud/work/frontier/tools/trace/udiff.py drone_collision_avoid --summary
