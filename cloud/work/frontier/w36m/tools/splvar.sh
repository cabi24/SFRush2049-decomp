#!/bin/sh
# splvar.sh FILE.c LABEL: unit-score func_80100E58 with FILE (E58 uargs), snapshot, run the split oracle and print
# the text-loop cols piece growth decisions (the split whose sym is the cols variable at T-64 is found by the
# second split of the lr that starts in loop 2; we print every split head + its rejects).
cd /home/cburnes/projects/rush2049-decomp
export TAG=w36m
F=$1; L=$2
UARGS="$(cat cloud/work/frontier/w33a/f0e58/uargs.txt) --internal rs_DrawEllipseText --internal rs_SetJust $XU" cloud/work/frontier/tools/trace/us.sh func_80100E58 $F 2>&1 | tail -1
cloud/work/frontier/tools/trace/snap.sh $L >/dev/null 2>&1
cloud/work/frontier/w36m/tools/oracle.sh $L func_80100E58 "" --summary >/dev/null 2>&1
awk '/split lr=/{h=$0} /reject/{print h" :: "prev} {prev=$0}' build/trace/w36m/or_$L.spl | head -12
