#!/bin/sh
# usage: bu.sh NAME file.c [file.c...] -- blob_unit score each (tag w9d), print one line each
cd /home/cburnes/projects/rush2049-decomp
name=$1; shift
for f in "$@"; do
  r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag w9d score $name --with "$f" 2>&1 | grep -E "EQUAL|FAIL|rror" | head -1)
  echo "$(basename $f): $r"
done
