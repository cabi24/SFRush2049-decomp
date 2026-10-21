#!/bin/sh
c=$(readlink -f $1); shift
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w37b score replay_save_prompt --with $c --internal replay_save_prompt --internal op_gfx_lock --internal op_gfx_unlock --internal op_HAL_SetFont --internal op_setButtonColor --internal op_ZOID_SetColorP --internal op_calcOptionMenuMaxSize --internal op_initButtons "$@" 2>&1 | grep -v "defined by" | cut -c1-250
grep -A12 "^  replay_save_prompt" build/blob_unit/w37b/umerge.log | awk 'NR>1 && /^  [^ ]/{exit} {print}' | tr '\n' ' '; echo
