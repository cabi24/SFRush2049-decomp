#!/bin/sh
# w25f full proof (builder, scratch root; w23b/w19a recipe): IDO 7.1 context rows + LOCK check for every
# lane/sources/*.c; the TU edit (tools/tu_edits.py) alone proven neutral; then all sources spliced
# (promote_batch --prefix-dedup replica, tools/splice.py) into the EDITED TU, built with IDO 7.1 through
# asm-processor and compared with the current TU (tools/tusplice.sh); TU-context check against the edited TU.
set -u
S=lane/sources
r=""; for f in $(ls $S | sed "s/\.c$//"); do r="$r $f=$S/$f.c"; done
python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 lane/ctx71_O2.jsonl $r
rm -rf spl tue; python3 lane/tools/tu_edits.py tue/src/rom
mkdir -p spl/src/rom; cp tue/src/rom/*.c spl/src/rom/
echo "== TU edit alone"; sh lane/tools/tusplice.sh $(ls tue/src/rom | sed 's/\.c$//')
for f in $(ls $S | sed 's/\.c$//'); do
  tu=$(grep -l "GLOBAL_ASM(\"asm/us/nonmatchings/rom/[^/]*/$f.s\")" tue/src/rom/*.c | head -1)
  TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557 \
  python3 lane/tools/check.py $S/$f.c $f $f --tu $tu --tu-flags "-G 0 -mips2 -O2 -non_shared -I include -I include/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -I src/rom" --ctx lane/ctx71_O2.jsonl 2>/dev/null | grep -E "^(==|LOCK:|TU:|RESULT)" | cut -c1-200 | tr '\n' ' '; echo; done
TUSRC=tue/src/rom python3 lane/tools/splice.py lane/ctx71_O2.jsonl spl/src/rom $(ls $S | sed 's/\.c$//')
echo "== all sources spliced (edited TU)"; sh lane/tools/tusplice.sh $(ls spl/src/rom | sed 's/\.c$//')
