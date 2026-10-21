#!/bin/sh
# usage: u.sh cand.c [extra args]  -- unit score of replay_save_prompt (tag w37b)
c=$1; shift
python3 -m tools.conveyor.pipeline.blob_unit --tag w37b score replay_save_prompt --with $c --internal replay_save_prompt --internal op_gfx_lock --internal op_gfx_unlock --internal op_font_set --internal op_SetFont --internal op_setButtonColor --internal op_calcOptionMenuMaxSize --internal op_initButtons "$@" 2>&1 | grep -vE "^\s+\+0x" | cut -c1-260
grep -A12 "^  replay_save_prompt" build/blob_unit/w37b/umerge.log | awk 'NR>1 && /^  [^ ]/{exit} {print}' | tr '\n' ' '; echo
