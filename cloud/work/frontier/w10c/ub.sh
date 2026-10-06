#!/bin/bash
# usage: ub.sh NAME body1.c body2.c ... : unit-score each body for NAME, print "file: N differ" (+ aligned rows)
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w10c
fn=$1; shift
for b in "$@"; do
  r=$($W/us.sh $b $fn | grep -E "FAIL|EQUAL|error|rror" | head -1)
  a=$(cd /home/cburnes/projects/rush2049-decomp && python3 $W/udiff.py $fn 2>/dev/null | tail -1)
  echo "$b: $r | $a"
done
