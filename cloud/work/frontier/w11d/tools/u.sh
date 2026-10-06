#!/bin/sh
# usage: u.sh CAND.c NAME [blob_unit score args...] -- score in unit (tag w11d), then aligned diff of NAME
cd /home/cburnes/projects/rush2049-decomp
c=$1; n=$2; shift; shift
python3 -m tools.conveyor.pipeline.blob_unit --tag w11d score $n "$@" --with "$c" 2>&1 | grep -E "EQUAL|FAIL|rror|differ in" 
python3 cloud/work/frontier/w11d/tools/udiff.py $n
