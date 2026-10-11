#!/bin/sh
# w19a (builder, scratch root): LOCK-score each target's D10 source under IDO 7.1
for f in $(cat lane/targets.txt); do
  p=$(sh lane/tools/d10.sh $f)
  if [ -z "$p" ] || [ ! -f "$p" ]; then echo "== $f no-d10-source"; continue; fi
  printf "== %s %s " $f $p; sh lane/tools/sc.sh $p $f 2>&1 | grep -E "^LOCK:" | head -1 || echo
done
