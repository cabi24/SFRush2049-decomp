#!/bin/sh
# run.sh cand.c FN [FN...] [-- extra blob_unit args]: unit score (tag w10d) + aligned row counts per FN
cd /home/cburnes/projects/rush2049-decomp
c=$(realpath "$1"); shift
fns=""; extra=""
while [ $# -gt 0 ]; do if [ "$1" = "--" ]; then shift; extra="$*"; break; fi; fns="$fns $1"; shift; done
python3 -m tools.conveyor.pipeline.blob_unit --tag w10d score $fns --with "$c" $extra 2>&1 | grep -E "EQUAL|FAIL|inlined|differ in"
for f in $fns; do printf '%s: ' $f; python3 cloud/work/frontier/w10d/tools/udiff.py $f | tail -1; done
