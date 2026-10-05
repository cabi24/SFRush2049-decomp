#!/bin/sh
# uv.sh NAME FILE.c... -- unit score each variant (tag w6b), print the result line; EXTRA= for extra args
N=$1; shift
for f in "$@"; do
  p=$(realpath "$f")
  r=$(cd /home/cburnes/projects/rush2049-decomp && python3 -m tools.conveyor.pipeline.blob_unit --tag w6b score $N $EXTRA --with "$p" 2>&1 | grep -E "$N:|rror" | head -1)
  echo "$(basename $f): $r"
done
