#!/bin/sh
# q.sh FN FILE.c... : us3 + aligned row counts (words/ops/norm) for FN
F=$1; shift
D=/home/cburnes/projects/rush2049-decomp
for f in "$@"; do
  r=$($D/cloud/work/frontier/w26c/f308/us3.sh $f 2>&1 | grep -E "(EQUAL|FAIL) $F|locked" | sed 's/^ *//' | cut -c1-70 | tr '\n' ' ')
  w=$(cd $D && TAG=w26c python3 cloud/work/frontier/tools/trace/udiff.py $F --summary | sed 's/.*differing rows \([0-9]*\).*frame \([0-9/]*\).*/\1 fr \2/')
  o=$(cd $D && TAG=w26c python3 cloud/work/frontier/tools/trace/udiff.py $F --summary --ops | sed 's/.*differing rows \([0-9]*\).*/\1/')
  n=$(cd $D && TAG=w26c python3 cloud/work/frontier/tools/trace/udiff.py $F --summary --norm | sed 's/.*differing rows \([0-9]*\).*/\1/')
  echo "$(basename $f): $r| words $w ops $o norm $n"
done
