#!/bin/bash
# tt.sh BODY LABEL NAME [score args] : unit score (us.sh) then rerun temp-tracing uopt on the snapshot; prints [TMP] spill lines for procs whose f_spilltemps matches GREP (default all)
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11b
b=$1; lab=$2; shift 2
$W/tools/us.sh $b "$@" | grep -E "EQUAL|FAIL|rror" | head -3
ssh watchman2 "cd ~/rush2049/scratch/frontier/w11b && rm -rf st_$lab && mkdir st_$lab && cp ../unit/w11b/stage/merged ../unit/w11b/stage/st st_$lab/ && cd st_$lab && cp st st.run && TMPLOG=1 ../uopt2/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt -t st.run optlog 2>tmp.log >/dev/null; cmp -s opt ../../unit/w11b/stage/opt && echo opt-identical; wc -l < tmp.log"
