#!/bin/sh
# ON THE BUILDER: altrace_remote.sh SCR LABEL NAME [ENV=V..] -> traced uopt_al on st_LABEL; NAME's records to st_LABEL/al_NAME.log
S=$1; L=$2; N=$3; shift 3
o=$(sh $S/tk/tk.sh $S $L ord $N) || exit 1
cd $S/st_$L && cp st st.al && env ALTRACE=1 "$@" $S/bin/uopt_al -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.al -t st.al optlog 2>al.log
awk -v k=$((o+1)) '/^PROC/{n++} n==k' al.log > al_$N.log; echo "$N ordinal $o -> $(wc -l < al_$N.log) lines"
