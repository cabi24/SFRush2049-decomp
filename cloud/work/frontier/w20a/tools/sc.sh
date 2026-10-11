#!/bin/sh
# w18c (builder, scratch root): LOCK+TU score of SRC FN under IDO 7.1 (toolkit 4586d230), w16g check.py.
# usage: sh lane/tools/sc.sh SRC FN [extra check.py args]
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
src=$1; fn=$2; shift 2
s=$(grep -l "GLOBAL_ASM(\"asm/us/nonmatchings/rom/[^/]*/$fn.s\")" src/rom/*.c | head -1)
O2TU="-G 0 -mips2 -O2 -non_shared -I include -I include/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -I src/rom"
TK=$TK python3 lane/tools/check.py $src $fn $fn ${s:+--tu $s} --tu-flags "$O2TU" "$@"
