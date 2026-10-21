#!/bin/bash
# darea.sh LABEL GROUPC : drawView area when kept separately
cd /home/cburnes/projects/rush2049-decomp
export TAG=w36f; T=cloud/work/frontier/tools/trace
$T/snap.sh $1 $2 func_803AD57C,drawView >/dev/null 2>&1
$T/spill.sh $1 drawView 2>&1 | grep -E "f_readnxtinst"
