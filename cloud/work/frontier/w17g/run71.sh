#!/bin/sh
# w17g replay under IDO 7.1 (builder, scratch root; see run_all.sh for the layout). For every source in
# lane/sources: context row with IDO 7.1 (ctx71.py -> lane/ctx/ctx71_O2.jsonl), then w16g check.py with TK=tk71:
# LOCK (reduced TU) and TU (real src/rom TU text before the slot + ctx preamble + body), both against the
# reloc-aware inventory target.
set -u
S=lane/sources
O2TU="-G 0 -mips2 -O2 -non_shared -I include -I include/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -I src/rom"
F="${F:-$(ls $S | sed 's/\.c$//')}"   # set F (and PART=1) to rerun a subset without rewriting the full ctx file
r=""; for f in $F; do r="$r $f=$S/$f.c"; done
C=lane/ctx/ctx71_O2.jsonl; [ -n "${PART:-}" ] && C=lane/ctx/part.jsonl
python3 lane/ctx71.py $C $r
tu() { s=$(grep -l "GLOBAL_ASM(\"asm/us/nonmatchings/rom/[^/]*/$1.s\")" src/rom/*.c | head -1); [ -n "$s" ] && echo "--tu $s"; }
for f in $F; do TK=$PWD/tk71 python3 lane/check.py $S/$f.c $f $f $(tu $f) --ctx $C --tu-flags "$O2TU"; done
