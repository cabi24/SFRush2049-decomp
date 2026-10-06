#!/bin/sh
# usage: u.sh NAME file.c [file.c...] [-- extra blob_unit args] -- whole-program unit score summary per file
cd /home/cburnes/projects/rush2049-decomp
name=$1; shift
for f in "$@"; do
  r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag w10a score $name $UEXTRA --with $f 2>&1 | grep -E "FAIL|EQUAL|MATCH|error|Error" | head -3 | tr '\n' ' ')
  echo "$f: $r"
done
