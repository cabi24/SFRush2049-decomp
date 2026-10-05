#!/bin/sh
# usage: ubatch.sh DIR "NAMES" [extra blob_unit score args] -- score each DIR/*.c in the whole-program unit (tag w3c)
dir="$1"; names="$2"; shift; shift
cd /home/cburnes/projects/rush2049-decomp
for f in "$dir"/*.c; do
  r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag w3c score $names --with "$f" "$@" 2>&1 | grep -E "^  (FAIL|EQUAL|OK|PASS)|equal;" | sed -E 's/^  //; s/: ([0-9]+) of [0-9]+ words differ.*/:\1/' | tr '\n' ' ')
  echo "$(basename $f): $r"
done
