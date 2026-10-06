#!/bin/sh
# usage: ub.sh CAND.c NAME [blob_unit args] -- unit score + aligned diff (tag w10f)
D=$(pwd); cd /home/cburnes/projects/rush2049-decomp
c=$(cd $D; realpath $1); n=$2; shift; shift
python3 -m tools.conveyor.pipeline.blob_unit --tag w10f score $n --with "$c" "$@" 2>&1 | grep -E "EQUAL|FAIL|rror|broken" | head -3
python3 cloud/work/frontier/w10f/tools/udiff.py $n --obj build/blob_unit/w10f/unit.o $UD
