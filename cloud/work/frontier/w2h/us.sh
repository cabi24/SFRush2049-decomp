#!/bin/sh
# usage: us.sh NAME cand.c [extra blob_unit args] -- score in the whole-program unit, print summary + diff row count
name="$1"; src="$2"; shift; shift
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w2h score $name --with "$src" "$@" 2>&1 | grep -v "^       +"
python3 cloud/work/frontier/w2h/udis.py $name | tail -1
