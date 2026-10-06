#!/bin/sh
# vu.sh NAME "extra blob_unit args" FILE... -- unit score + aligned row count per file (tag w11a)
R=/home/cburnes/projects/rush2049-decomp
name=$1; args=$2; shift; shift; W=$(pwd)
for f0 in "$@"; do f=$(cd $W; realpath $f0); cd $R
  r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag w11a score $name $args --with $f 2>&1 | grep -E "FAIL|EQUAL|rror" | head -3 | cut -c1-100 | tr '\n' ' ')
  d=$(python3 cloud/work/frontier/w11a/tools/udiff.py $name --obj build/blob_unit/w11a/unit.o 2>&1 | tail -1)
  echo "$(basename $f): $d :: $r"
done
