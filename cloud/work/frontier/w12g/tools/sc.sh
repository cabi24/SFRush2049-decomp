#!/bin/bash
# usage: sc.sh NAME FILE.c...  -- unit score (tag w12g) with the direct-return func_800B61A8 in place
cd /home/cburnes/projects/rush2049-decomp
n=$1; shift
for f in "$@"; do
  r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag w12g score $n --with cloud/work/frontier/w12g/func_800B61A8.c --with $f $XARGS --neighbours 2>&1 | grep -E "(EQUAL|FAIL) $n|differ in this|rror" | tr '\n' ' ' | cut -c1-200)
  echo "$(basename $f): $r"
done
