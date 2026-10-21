#!/bin/bash
# area.sh LABEL GROUPDIR : cfe area + homes of func_803AD57C
cd /home/cburnes/projects/rush2049-decomp
export TAG=w36f; T=cloud/work/frontier/tools/trace
$T/snap.sh $1 cloud/work/frontier/w36f/func_803AD57C/$2/group.c func_803AD57C >/dev/null 2>&1
$T/spill.sh $1 func_803AD57C 2>&1 | grep -E "f_readnxtinst|\[TMP\] spill" | sed 's/kind=4 size=4 //' | tr '\n' ' '; echo
