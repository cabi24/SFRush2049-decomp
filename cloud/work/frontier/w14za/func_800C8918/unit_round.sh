#!/bin/sh
# usage: unit_round.sh DIR OUT  -- unit-scores every DIR/*.c with blob_unit (tag w14za), one line per file
cd /home/cburnes/projects/rush2049-decomp
: > "$2"
for f in "$1"/v*.c; do
  r=$(timeout 300 python3 -m tools.conveyor.pipeline.blob_unit --tag w14za score func_800C8918 --with "$f" 2>&1 | grep -E "^ *(FAIL|EQUAL) func_800C8918" | head -1)
  echo "$(basename $f) $r" >> "$2"
done
echo ROUND_DONE >> "$2"
