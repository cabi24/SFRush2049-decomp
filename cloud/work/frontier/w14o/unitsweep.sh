#!/bin/sh
# unitsweep.sh NAME DIR OUT : unit-score every DIR/v*.c with blob_unit (tag w14o), one line per file to OUT
name=$1; dir=$(realpath "$2"); out=$(realpath -m "$3"); : > "$out"
cd /home/cburnes/projects/rush2049-decomp || exit 1
for f in "$dir"/v*.c; do
  r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag w14o --jobs 2 score $name --with "$f" --neighbours 2>&1 | grep -E "(EQUAL|FAIL) $name:" | head -1)
  echo "$(basename $f) $r" >> "$out"
done
echo DONE >> "$out"
