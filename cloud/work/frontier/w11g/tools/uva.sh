#!/bin/sh
# uva.sh NAME FILE.c... -- unit score each variant (tag w7d) + aligned rows; EXTRA= for extra args
N=$1; shift
for f in "$@"; do
  p=$(realpath "$f")
  r=$(cd /home/cburnes/projects/rush2049-decomp && python3 -m tools.conveyor.pipeline.blob_unit --tag w11g score $N $EXTRA --with "$p" 2>&1 | grep -E "$N:|rror" | head -1 | sed 's/^ *//')
  a=$(cd /home/cburnes/projects/rush2049-decomp && python3 cloud/work/frontier/w3a/tools/udiff.py $N --obj build/blob_unit/w7d/unit.o 2>&1 | tail -1 | grep -o "differing rows [0-9]*")
  echo "$(basename $f): $r | $a"
done
