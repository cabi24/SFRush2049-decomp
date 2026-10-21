#!/bin/sh
c=$(readlink -f $1); shift
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w37b score drone_collision_avoid --with $c --internal drone_collision_avoid --internal du_gfx_lock --internal du_gfx_unlock --internal du_font_set --internal du_SetFont --internal du_HAL_SetFont --internal du_validOption "$@" 2>&1 | grep -v "defined by" | cut -c1-250
grep -A12 "^  drone_collision_avoid" build/blob_unit/w37b/umerge.log | awk 'NR>1 && /^  [^ ]/{exit} {print}' | tr '\n' ' '; echo
