#!/bin/sh
# sc.sh FILE.c [FN]  -- unit score with diff (TAG=w15d)
cd /home/cburnes/projects/rush2049-decomp
export TAG=w15d
FN=${2:-func_800A79F4}
DIFF=1 cloud/work/frontier/tools/trace/us.sh $FN "$1" 2>&1 | grep -v '^   '
