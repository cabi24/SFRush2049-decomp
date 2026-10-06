#!/bin/sh
# run.sh cand.c FN [FN...] [-- extra blob_unit args]: unit score (tag w11c) + aligned row counts per FN
D0=$(pwd); cd /home/cburnes/projects/rush2049-decomp
c=$(cd $D0; realpath "$1"); shift
fns=""; extra=""
while [ $# -gt 0 ]; do if [ "$1" = "--" ]; then shift; extra="$*"; break; fi; fns="$fns $1"; shift; done
python3 -m tools.conveyor.pipeline.blob_unit --tag w11c score $fns --with "$c" $extra 2>&1 | grep -E "EQUAL|FAIL|inlined|differ in"
for f in $fns; do printf '%s: ' $f; python3 cloud/work/frontier/w11c/tools/udiff.py $f | tail -1; done
