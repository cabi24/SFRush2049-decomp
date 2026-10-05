#!/bin/sh
# usage: us.sh cand.c NAME [blob_unit args] -- blob_unit score (tag w4d) + aligned diff
D=$(pwd); cd /home/cburnes/projects/rush2049-decomp
src="$(cd $D; realpath $1)"; name="$2"; shift; shift
python3 -m tools.conveyor.pipeline.blob_unit --tag w4d score $name --with "$src" "$@" 2>&1 | grep -v "^       +0x" | grep -E "EQUAL|FAIL|rror"
python3 cloud/work/frontier/w3a/tools/udiff.py $name --obj build/blob_unit/w4d/unit.o
