#!/bin/sh
# usage: [TAG=w6a] us.sh cand.c NAME [udiff args] -- blob_unit score + aligned diff
D=$(pwd); cd /home/cburnes/projects/rush2049-decomp
T=${TAG:-w6a}
src="$(cd $D; realpath $1)"; name="$2"; shift; shift
python3 -m tools.conveyor.pipeline.blob_unit --tag $T score $name --with "$src" 2>&1 | grep -v "^       +0x" | grep -E "EQUAL|FAIL|rror"
python3 cloud/work/frontier/w6a/tools/udiff.py $name --obj build/blob_unit/$T/unit.o "$@"
