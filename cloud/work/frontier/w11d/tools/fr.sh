#!/bin/sh
# fr.sh NAME FILE... : unit score + frame row + sp-relative addiu rows (ours) for each file
cd /home/cburnes/projects/rush2049-decomp
n=$1; shift
for f in "$@"; do
 r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag w11d score $n --with "$f" 2>&1 | grep -E "(FAIL|EQUAL) $n" | head -1 | sed 's/^ *//; s/; +0x.*//')
 d=$(python3 cloud/work/frontier/w11d/tools/udiff.py $n --all)
 fr=$(echo "$d" | grep -m1 "addiu sp,sp" | awk '{print $NF}')
 sp=$(echo "$d" | grep -oE "addiu (a0|s[0-9]),sp,[0-9]+ *$" | sort -u | tr '\n' ' ')
 rows=$(echo "$d" | tail -1)
 echo "$(basename $f): $r | frame $fr | $sp| $rows"
done
