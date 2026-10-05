#!/bin/bash
# var.sh <group> <partsdir> <partfile> <FN> <variant.c...> : score each variant placed as partfile; prints summary line + frame
g=$1; d=$2; pf=$3; fn=$4; shift 4
for v in "$@"; do
  cp $v $d/$pf
  r=$(./mk.sh $g $d $fn 2>&1)
  echo "$(basename $v): $(echo "$r" | tail -1) | $(echo "$r" | grep -m1 'addiu sp,sp' )"
done
