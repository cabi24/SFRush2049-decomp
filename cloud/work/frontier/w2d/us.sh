#!/bin/sh
# usage: us.sh cand.c NAME [objdiff args] -- score NAME in the whole-program unit and print the aligned diff
src="$1"; name="$2"; shift; shift
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w2d score $name --with "$src" 2>&1 | grep -v "^       +0x" | tail -6
python3 cloud/work/frontier/w2d/objdiff.py build/blob_unit/w2d/unit.o $name "$@"
