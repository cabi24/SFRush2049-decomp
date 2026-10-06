#!/bin/sh
# frun.sh NAME... : runs/f/NAME.c is a variant of dev/part_f058.c; build the group and print func_8009F058's
# score, frame and t0 slot (from the mismatch lines; blank = equal to retail)
cd $(dirname $0)/..
for v in "$@"; do
  d=runs/f/$v; mkdir -p $d
  sh dev/build.sh $d/group.c dev/part_ipgp.c dev/tail.c >/dev/null
  python3 - $d/group.c runs/f/$v.c <<'PY'
import sys
g=open(sys.argv[1]).read(); base=open('dev/part_f058.c').read(); new=open(sys.argv[2]).read()
assert base in g; open(sys.argv[1],'w').write(g.replace(base,new))
PY
  cp runs/g1/group.json $d/
  tools/grp.sh $d > $d/score.txt 2>&1
  r=$(grep -A2000 "^func_8009F058:" $d/score.txt | grep -m1 -E "MATCH|differ|rror")
  fr=$(grep -A2000 "^func_8009F058:" $d/score.txt | grep -m1 "addiu sp,sp,-" | sed 's/.*got [0-9a-f]* //')
  t0=$(grep -A2000 "^func_8009F058:" $d/score.txt | grep -m1 "sw t0" | sed 's/.*got [0-9a-f]* //')
  echo "$v: $r | $fr | $t0"
done
