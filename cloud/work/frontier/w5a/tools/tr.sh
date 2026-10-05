#!/bin/sh
# tr.sh DIR OUT [env...]: run traced uopt on DIR/merged (st restored each time)
d=$1; o=$2; shift; shift
cd $d && cp st st.run && env CDX_LOG=1 CDX_OUT=$o "$@" ../uopt/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.x -t st.run optlog >/dev/null 2>&1
