#!/bin/sh
# area.sh LABEL FILE.c : unit-build FILE (camera_update), snapshot, print Udef/spill/gettemp area steps + homes
D0=$(pwd); cd /home/cburnes/projects/rush2049-decomp
export TAG=w12f; T=cloud/work/frontier/tools/trace
f=$(cd "$D0"; realpath "$2"); cloud/work/frontier/w12f/tools/fl.sh ${FN:-camera_update} "$f" | tail -1
$T/snap.sh $1 >/dev/null 2>&1
$T/spill.sh $1 ${FN:-camera_update} 2>&1 | grep -E "AREA f_(readnxt|spill|gettemp)|TMP\] spill web" | tr '\n' ' ' | sed 's/AREA /\nAREA /g'; echo
