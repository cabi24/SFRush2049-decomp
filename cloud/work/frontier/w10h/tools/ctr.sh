#!/bin/bash
# usage: ctr.sh LABEL PROC -- CDX colouring trace (w3a traced uopt) for one proc of snapshot st_LABEL; prints sum.sh summary to stdout, raw in st_LABEL/d.txt
ssh watchman2 "cd ~/rush2049/scratch/frontier/w10h/st_$1 && cp st st.run && env CDX_LOG=1 CDX_OUT=\$PWD/d.txt CDX_PROC=$2 CDX_DETAIL_WEB=all ../uopt/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.x -t st.run optlog >/dev/null 2>&1; cat d.txt" | sh /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w3a/tools/sum.sh /dev/stdin
