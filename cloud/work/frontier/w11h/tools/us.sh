#!/bin/sh
# us.sh NAME CAND.c [extra blob_unit args] -- score in unit (tag w11h) and print aligned diff
cd /home/cburnes/projects/rush2049-decomp
n=$1; c=$2; shift 2
python3 -m tools.conveyor.pipeline.blob_unit --tag w11h --jobs 2 score $n --with $c "$@" 2>&1 | grep -E "EQUAL|FAIL|locked bodies" 
python3 cloud/work/frontier/w11h/tools/udiff.py $n --obj build/blob_unit/w11h/unit.o --all 2>&1 | grep -E "^\||want"
