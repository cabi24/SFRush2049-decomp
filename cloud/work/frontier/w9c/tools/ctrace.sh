#!/bin/sh
# usage: ctrace.sh CAND.c LABEL [PROC] -- (adapted from w3a/tools/ctrace.sh, tag w9c, copy of w3a's traced uopt)
D=$(pwd); cd /home/cburnes/projects/rush2049-decomp
cand=$(cd $D; realpath $1); lab=$2; proc=$3
python3 -m tools.conveyor.pipeline.blob_unit --tag w9c --jobs 2 score func_8008E408 --with "$cand" 2>&1 | grep -E "EQUAL|FAIL|differ" | head -1 | cut -c1-120
ssh watchman2 "cd ~/rush2049/scratch/frontier/w9c && rm -rf st_$lab && mkdir st_$lab && cp ../unit/w9c/stage/merged ../unit/w9c/stage/st st_$lab/"
if [ -z "$proc" ]; then
  ssh watchman2 "cd ~/rush2049/scratch/frontier/w9c/st_$lab && cp st st.run && env CDX_LOG=1 CDX_OUT=\$PWD/all.txt ../uopt/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.x -t st.run optlog >/dev/null 2>&1; echo traced; wc -l all.txt"
else
  ssh watchman2 "cd ~/rush2049/scratch/frontier/w9c/st_$lab && cp st st.run && env CDX_LOG=1 CDX_OUT=\$PWD/d.txt CDX_PROC=$proc CDX_DETAIL_WEB=all ../uopt/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.x -t st.run optlog >/dev/null 2>&1" 
  ssh watchman2 "cat ~/rush2049/scratch/frontier/w9c/st_$lab/d.txt" | sh /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w3a/tools/sum.sh /dev/stdin
fi
