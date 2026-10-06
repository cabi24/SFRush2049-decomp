#!/bin/sh
# usage: [TAG=w10b] us.sh cand.c NAME [udiff args] -- blob_unit score + aligned diff
D=$(pwd); cd /home/cburnes/projects/rush2049-decomp
T=${TAG:-w10b}
src="$(cd $D; realpath $1)"; name="$2"; shift; shift
python3 -m tools.conveyor.pipeline.blob_unit --tag $T score $name --with "$src" 2>&1 | grep -v "^       +0x" | grep -E "EQUAL|FAIL|rror"
python3 cloud/work/frontier/w10b/tools/udiff.py $name --obj build/blob_unit/$T/unit.o "$@"
