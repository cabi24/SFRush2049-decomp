#!/bin/sh
# v.sh MODE.c [LABEL]: score the w13a component with MODE.c in place of comp/mode.c; print E05F0 line, aligned rows,
# and (with LABEL) the $8 alias directives of E05F0's ugen listing.
M=$(realpath $1); cd /home/cburnes/projects/rush2049-decomp
C=cloud/work/frontier/w13a/comp
python3 -m tools.conveyor.pipeline.blob_unit --tag w13a score \
  func_800D5E64 best_times_display mode_select_handler func_800DED78 func_800DEF60 func_800DFBA0 mode_select_input func_800E0048 func_800E0050 func_800E05F0 \
  --with $C/d5e64.c --with $M --with $C/msh.c --with $C/ded78.c \
  --internal best_times_display --internal mode_select_input --internal func_800DFBA0 \
  --internal mode_select_handler --internal func_800DED78 --internal func_800E0050 --internal func_800DEF60 --internal func_800E0048 --neighbours > /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/d8205ce0-e823-439c-818f-b2eb696f34bd/scratchpad/v.out 2>&1
grep -E "EQUAL|FAIL|locked bodies|score:|rror" /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/d8205ce0-e823-439c-818f-b2eb696f34bd/scratchpad/v.out | grep -v "^  EQUAL" | cut -c1-150
TAG=w13a python3 cloud/work/frontier/tools/trace/udiff.py func_800E05F0 | tail -1
TAG=w13a python3 cloud/work/frontier/tools/trace/udiff.py func_800E05F0 --ops | tail -1
if [ -n "$2" ]; then
  cloud/work/frontier/w13a/tools/lst.sh $2 func_800E05F0 > cloud/work/frontier/w13a/var/lst_$2.s 2>/dev/null
  echo "   A8: $(awk '/alias\t\$8,\$sp/{print "[" prev " | " $0 "]"} !/^\t\.(loc|livereg)/{prev=$0}' cloud/work/frontier/w13a/var/lst_$2.s | tr '\t' ' ' | tr '\n' ' ')"
fi
