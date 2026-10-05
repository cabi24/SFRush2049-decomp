#!/bin/sh
# usage: us.sh cand.c NAME [udiff args] -- score NAME in the whole-program unit with cand.c, then aligned diff
cd /home/cburnes/projects/rush2049-decomp
src="$1"; name="$2"; shift; shift
python3 -m tools.conveyor.pipeline.blob_unit --tag w2c score $name --with "$src" 2>&1 | grep -v "^       +0x" | tail -6
python3 cloud/work/frontier/w2c/udiff.py $name "$@"
