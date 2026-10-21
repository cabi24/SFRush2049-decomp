#!/bin/bash
# runv.sh FILE... : score each Resurrect-cluster variant in the unit, print summary lines
cd /home/cburnes/projects/rush2049-decomp
for f in "$@"; do
  r=$(cloud/work/frontier/w36s/bu.sh $f | grep -E "^  (EQUAL|FAIL) (stunt_combo_display|func_800D348C|func_800C4F68)" | cut -c1-80 | tr '\n' ' ')
  s=$(TAG=w36s python3 cloud/work/frontier/tools/trace/udiff.py stunt_combo_display --summary)
  echo "$(basename $f): $r | $s"
done
