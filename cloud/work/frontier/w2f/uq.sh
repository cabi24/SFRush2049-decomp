#!/bin/sh
# usage: [UARGS="--internal X"] uq.sh file.c NAME [udiff args] -- score in the whole-program unit and show aligned diff
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w2f score $2 --with $1 $UARGS 2>&1 | grep -E "EQUAL|FAIL|rror|cc:|cfe|differ in this" | head -8
f="$1"; n="$2"; shift; shift
python3 cloud/work/frontier/w2f/udiff.py $n "$@"
