#!/bin/sh
# usage: u.sh CAND.c NAME [blob_unit score args...] -- score in unit (tag w10e), then aligned diff of NAME
cd /home/cburnes/projects/rush2049-decomp
c=$1; n=$2; shift; shift
python3 -m tools.conveyor.pipeline.blob_unit --tag w10e score $n "$@" --with "$c" 2>&1 | grep -E "EQUAL|FAIL|rror|differ in" 
python3 cloud/work/frontier/w10e/tools/udiff.py $n
