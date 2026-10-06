#!/bin/bash
# usage: tmptrace.sh CAND.c LABEL -- score in unit, snapshot stage, rerun temp-tracing uopt; prints [TMP] lines of the proc that has the most temps matching
T=$(dirname $(realpath $0)); lab=$2
$T/sc.sh $1 | head -1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w10h && rm -rf st_$lab && mkdir st_$lab && cp ../unit/w10h/stage/merged ../unit/w10h/stage/st st_$lab/ && cd st_$lab && cp st st.run && TMPLOG=1 ../uopt2/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt -t st.run optlog 2>tmp.log >/dev/null; cmp opt ../../unit/w10h/stage/opt && echo opt-identical; wc -l < tmp.log"
