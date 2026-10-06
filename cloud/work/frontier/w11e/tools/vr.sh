#!/bin/sh
# usage: vr.sh A.c B.c ... -- score each in the trio unit, print engine_torque_calc/trg/esu summary rows
for f in "$@"; do
  r=$(/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11e/tools/s3.sh $f | grep "differing rows" | sed 's/.*differing rows \([0-9]*\).*/\1/' | tr '\n' ' ')
  echo "$f: $r"
done
