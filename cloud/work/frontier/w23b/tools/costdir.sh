#!/bin/sh
# w23b (Pi): for each .c in lane-relative DIR: 5.3 callee cost + count of split type=1 webs; then 7.1 LOCK lines
d=$1; fn=$2
for f in $d/*.c; do k=$(basename $f .c); r=$(PAT='type=1|best=' sh tools/cost53.sh $f $fn c_$k 2>/dev/null)
  c=$(echo "$r" | grep -o 'best=[0-9.]*' | sort -t= -k2 -g | tail -1)
  s=$(echo "$r" | grep '^p1' | grep 'type=1' | grep -c split); s2=$(echo "$r" | grep '^p1' | grep 'type=1' | grep -c -- '-> s')
  echo "$k $c p1-split-addr=$s addr-in-s=$s2"; done
sh tools/vb.sh $d $fn
