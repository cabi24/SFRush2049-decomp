#!/bin/bash
# usage: st.sh CAND.c [LABEL] -- unit score + sp census + spill-temp allocation (temp-tracing uopt) for func_8008E408's proc
T=$(dirname $(realpath $0)); lab=${2:-$(basename $1 .c)}
$T/sc.sh $1 | head -1; $T/sp.sh | tail -1 | cut -c1-90
ssh watchman2 "cd ~/rush2049/scratch/frontier/w10h && rm -rf st_$lab && mkdir st_$lab && cp ../unit/w10h/stage/merged ../unit/w10h/stage/st st_$lab/ && cd st_$lab && cp st st.run && TMPLOG=1 ../uopt2/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt -t st.run optlog 2>tmp.log >/dev/null; awk '/procinit/{if(n>=5 && n<=14 && hit) print b; b=\"\"; n=0; hit=0} /spill web/{n++; sub(/\[TMP\] spill /,\"\"); b=b \" | \" \$0; next} /f_spilltemps/{if(\$0 ~ /disp (19|20)[0-9]->2[23]/) hit=1; b=b \" || \" \$4 \" \" \$5}' tmp.log"
