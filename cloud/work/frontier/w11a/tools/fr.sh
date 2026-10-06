#!/bin/sh
# fr.sh NAME TAG FILE... -- unit score + frame size + aligned rows for each file
name=$1; tag=$2; shift; shift; F=""; for x in "$@"; do F="$F $(realpath $x)"; done; set -- $F
cd /home/cburnes/projects/rush2049-decomp
for f in "$@"; do
  r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag $tag score $name --with "$f" 2>&1 | grep -E "EQUAL|FAIL|rror" | head -1 | cut -c1-90)
  d=$(python3 cloud/work/frontier/w11a/tools/udiff.py $name --obj build/blob_unit/$tag/unit.o --all 2>/dev/null); m=$(python3 cloud/work/frontier/w11a/tools/udiffm.py $name --obj build/blob_unit/$tag/unit.o --mnem 2>/dev/null | tail -1 | sed "s/.*differing rows \([0-9]*\).*/\1/")
  fr=$(echo "$d" | head -1 | awk '{print $NF}')
  rows=$(echo "$d" | tail -1 | sed 's/.*differing rows \([0-9]*\).*/\1/')
  echo "$(basename $f): frame=$fr rows=$rows mnem=$m | $r"
done
