#!/bin/sh
# v.sh MODEFILE.c [LABEL] : score the w13b component with MODEFILE as mode.c; print E05F0 verdict + word/ops rows + $8 alias lines
f=$(realpath $1); R=/home/cburnes/projects/rush2049-decomp; W=$R/cloud/work/frontier/w13b; cd $R
lab=${2:-$(basename $1 .c)}
MODE=$f TAG=w13b sh $W/comp/run.sh 2>&1 | grep -E "^  (EQUAL|FAIL)|locked bodies|score:" | sed 's/^/  /' | grep -v "EQUAL" 
echo "  rows: $(TAG=w13b python3 cloud/work/frontier/tools/trace/udiff.py func_800E05F0 --summary 2>&1 | grep -o 'differing rows [0-9]* ([a-z]*)\|got [0-9]*' | tr '\n' ' ') ops: $(TAG=w13b python3 cloud/work/frontier/tools/trace/udiff.py func_800E05F0 --ops --summary 2>&1 | grep -o 'differing rows [0-9]*')"
if [ -n "$AL" ]; then
  export TAG=w13b; T=cloud/work/frontier/tools/trace
  $T/snap.sh $lab >/dev/null 2>&1; $T/as1t.sh $lab func_800E05F0 >/dev/null 2>&1
  awk '/alias\t\$8,\$sp/{print "   [" NR ": " prev " | " $0 "]"} !/^\t\.(loc|livereg)/{prev=$0}' build/trace/w13b/fn_$lab.func_800E05F0.s | tr '\t' ' '
fi
