#!/bin/sh
# usage: gu.sh "score args (names + --internal..)" FN file.c [file.c...] -- unit score + aligned rows for FN
R=/home/cburnes/projects/rush2049-decomp
args="$1"; fn=$2; shift; shift
W=$(pwd)
for f0 in "$@"; do cd $W; f=$(realpath $f0); cd $R;
  r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag w10a score $args --with $f 2>&1 | grep -E "FAIL|EQUAL|rror" | tr '\n' ' ')
  d=$(python3 cloud/work/frontier/w3a/tools/udiff.py $fn --obj build/blob_unit/w10a/unit.o 2>&1 | tail -1)
  echo "$(basename $f): $d :: $r"
done
