#!/bin/bash
# df.sh MODE_VARIANT.c... : comp/ harness with comp/mode.c replaced by each variant; prints the func_800DFBA0 line
R=/home/cburnes/projects/rush2049-decomp; C=$R/cloud/work/frontier/w12a/comp; export TAG=w12a
F=(); for f in "$@"; do F+=("$(realpath $f)"); done; set -- "${F[@]}"; cd $R
for f in "$@"; do
  r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag w12a --jobs 2 score func_800DFBA0 mode_select_input func_800E05F0 func_800D5E64 best_times_display mode_select_handler func_800DED78 \
    --with $C/d5e64.c --with $(realpath $f) --with $C/msh.c --with $C/ded78.c \
    --internal best_times_display --internal mode_select_input --internal func_800DFBA0 --internal mode_select_handler --internal func_800DED78 --neighbours 2>&1 | grep -E '(EQUAL|FAIL) (func_800DFBA0|mode_select_input)|rror|locked bodies' | tr '\n' ' ')
  echo "$(basename $f): $r | rows $(python3 $R/cloud/work/frontier/tools/trace/udiff.py func_800DFBA0 --summary)"
done
