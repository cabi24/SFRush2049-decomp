#!/bin/sh
# t.sh FILE.c...: unit-score each (tag w21f) and print the low spill homes (sp < 120) of ours
F=""; for f in "$@"; do F="$F $(realpath $f)"; done
cd /home/cburnes/projects/rush2049-decomp
export TAG=w21f; T=cloud/work/frontier/tools/trace
for f in $F; do
  $T/us.sh entity_spawn_init $f | cut -c1-150
  echo "   homes: $($T/spcensus.sh entity_spawn_init | sed -n 4p | tr ' ' '\n' | awk -F, '$1>=80 && $1<200' | tr '\n' ' ')"
done
