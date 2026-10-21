#!/bin/bash
# sweep9058.sh OUT FILE... : one line per variant: name, D9058 verdict, udiff summary, D91A0 verdict
OUT=$1; shift
cd /home/cburnes/projects/rush2049-decomp
for f in "$@"; do
  r=$(D9058=$f cloud/work/frontier/w36k/tools/uu.sh cloud/work/frontier/w36k/groups/controlscr/caller.c 2>&1)
  d=$(echo "$r" | grep -E "func_800D9058" | head -1 | sed 's/^ *//')
  a=$(echo "$r" | grep -E "func_800D91A0" | head -1 | sed 's/^ *//' | cut -c1-40)
  echo "$(basename $f) | $d | $a" >> $OUT
done
echo DONE >> $OUT
