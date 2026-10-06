#!/bin/sh
# areas.sh NAME FILES... -- per variant: unit score line, named area (readnxtinst), final area after spilltemps, aligned rows
N=$1; shift
for f in "$@"; do
  r=$(sh /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w8b/tools/area.sh $N $f ar_tmp 2>&1)
  s=$(echo "$r" | head -1 | sed 's/^ *//')
  a=$(echo "$r" | grep f_readnxtinst | tail -1 | grep -o "area=[0-9]*")
  b=$(echo "$r" | grep f_spilltemps | tail -1 | grep -o "area=[0-9]*")
  c=$(cd /home/cburnes/projects/rush2049-decomp && python3 cloud/work/frontier/w3a/tools/udiff.py $N --obj build/blob_unit/w8b/unit.o 2>&1 | tail -1 | grep -o "differing rows [0-9]*")
  echo "$(basename $f): $s | named $a final $b | $c"
done
