#!/bin/bash
# ctr.sh BODY LABEL PROC [NAME] : unit score, snapshot merged ucode, traced colouring of PROC -> summary
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11b
b=$1; lab=$2; proc=$3; name=${4:-entity_update}
$W/tools/us.sh $b $name | grep -E "EQUAL|FAIL" | head -1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w11b/tr && rm -rf st_$lab && mkdir st_$lab && cp ../../unit/w11b/stage/merged ../../unit/w11b/stage/st st_$lab/ && cd st_$lab && cp st st.run && env CDX_LOG=1 CDX_OUT=\$PWD/d.txt CDX_PROC=$proc CDX_DETAIL_WEB=all ../uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.x -t st.run optlog >/dev/null 2>&1; sh ../sum.sh d.txt > sum.txt; wc -l < sum.txt"
