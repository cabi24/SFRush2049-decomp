#!/bin/bash
# ua.sh ANIM.c : unit score controls_animText + controls_animButtonText (installed controlscr group + candidate)
A=$(readlink -f $1); shift
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w38a score controls_animText controls_animButtonText "$@" --with $A \
  --internal controls_animButtonText --internal ct_gfx_lock --internal ct_gfx_unlock --internal ct_font_set \
  --internal ct_CenterText --internal ct_RightText --internal ct_SetFont --internal ct_touch 2>&1 | grep -E "EQUAL|FAIL|blob_unit score|rror|locked bodies" | cut -c1-120
for f in controls_animText controls_animButtonText; do TAG=w38a python3 cloud/work/frontier/tools/trace/udiff.py $f --summary; done
