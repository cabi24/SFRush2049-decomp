#!/bin/sh
# tr9.sh CAND.c LABEL [PROC] : score in unit (tag w9g), snapshot merged ucode into builder w9g/st_LABEL, run traced uopt
c=$(realpath "$1"); lab=$2; proc=$3
sh $(dirname $0)/am.sh "$c"
ssh watchman2 "cd ~/rush2049/scratch/frontier/w9g && rm -rf st_$lab && mkdir st_$lab && cp ../unit/w9g/stage/merged ../unit/w9g/stage/st st_$lab/ && cd st_$lab && cp st st.run && env CDX_LOG=1 CDX_OUT=\$PWD/d.txt ${proc:+CDX_PROC=$proc CDX_DETAIL_WEB=all} ../uopt/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.x -t st.run optlog >/dev/null 2>&1; cd ..; ${proc:+sh sum.sh st_$lab/d.txt}"
