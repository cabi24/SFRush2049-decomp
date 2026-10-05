#!/bin/sh
# usage: [TAG=w5a] us.sh cand.c NAME [udiff args] -- blob_unit score + aligned diff
D=$(pwd); cd /home/cburnes/projects/rush2049-decomp
T=${TAG:-w5a}
src="$(cd $D; realpath $1)"; name="$2"; shift; shift
python3 -m tools.conveyor.pipeline.blob_unit --tag $T score $name --with "$src" 2>&1 | grep -v "^       +0x" | grep -E "EQUAL|FAIL|rror"
python3 cloud/work/frontier/w5a/tools/udiff.py $name --obj build/blob_unit/$T/unit.o "$@"
