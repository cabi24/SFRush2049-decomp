#!/bin/sh
# usage: q.sh NAME "blob_unit args" FILE... -- one line per file: unit verdict + aligned differing rows
cd /home/cburnes/projects/rush2049-decomp
n=$1; args=$2; shift; shift
for f in "$@"; do
  r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag w10e score $n $args --with "$f" 2>&1 | grep -E "(FAIL|EQUAL) $n" | head -1 | sed 's/^ *//')
  d=$(python3 cloud/work/frontier/w10e/tools/udiff.py $n | tail -1)
  echo "$(basename $f): $r | $d"
done
