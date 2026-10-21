#!/bin/sh
# usage: area.sh FN FILE.c [FILE2.c ...] -- cfe named area (uopt f_readnxtinst AREA) + final area for FN in a one-file group
export TAG=w36x3
T=cloud/work/frontier/tools/trace
fn=$1; shift
for f in "$@"; do
  lab=ar$$
  $T/snap.sh $lab "$f" "$fn" >/dev/null 2>&1 || { echo "$f: stage failed"; continue; }
  a=$($T/spill.sh $lab "$fn" 2>&1 | grep -E "^AREA" | grep -v procinit | awk '{print $1" "$NF}' | sed 's/area=//' | awk 'NR==1{n=$2} {l=$2} END{print "named=" n " final=" l}')
  echo "$(basename $f): $a"
done
