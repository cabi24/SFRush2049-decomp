#!/bin/sh
# w20b full proof (builder, scratch root): IDO 7.1 ctx rows + LOCK/TU check for every lane/sources/*.c;
# TU edits alone == original; all sources spliced (promote_batch --prefix-dedup replica) into the
# edited TUs == original (IDO 7.1 both sides). Then the ownbss trio: LOCK lines + linked proof.
set -u
S=lane/sources
r=""; for f in $(ls $S | sed "s/\.c$//"); do r="$r $f=$S/$f.c"; done
python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 lane/ctx71_O2.jsonl $r | grep -v " ok$"
rm -rf ed spl tusrc; python3 lane/tools/tu_edits.py ed/src/rom >/dev/null
mkdir -p tusrc; cp src/rom/*.c tusrc/; cp ed/src/rom/*.c tusrc/
for f in $(ls $S | sed 's/\.c$//'); do
  s=$(grep -l "GLOBAL_ASM(\"asm/us/nonmatchings/rom/[^/]*/$f.s\")" tusrc/*.c | head -1)
  O2TU="-G 0 -mips2 -O2 -non_shared -I include -I include/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -I src/rom"
  TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557 \
    python3 lane/tools/check.py $S/$f.c $f $f --tu $s --tu-flags "$O2TU" --ctx lane/ctx71_O2.jsonl 2>/dev/null | grep -E "^(==|LOCK:|TU:|CTX)" | cut -c1-200 | tr '\n' ' '; echo; done
mkdir -p spl/src/rom; cp ed/src/rom/*.c spl/src/rom/
echo "== TU edits alone"; sh lane/tools/tusplice.sh $(ls ed/src/rom | sed 's/\.c$//')
rm -rf spl; mkdir -p spl/src/rom
TUSRC=tusrc python3 lane/tools/splice.py lane/ctx71_O2.jsonl spl/src/rom $(ls $S | sed 's/\.c$//') >/dev/null
echo "== all sources spliced (into the edited TUs)"; sh lane/tools/tusplice.sh $(ls spl/src/rom | sed 's/\.c$//')
echo "== ownbss trio (static .bss; context_check refuses .bss, manual splice)"
for f in func_80016E68 func_80016EE0 func_80016F80; do printf '%s ' $f; sh lane/tools/sc.sh lane/ownbss/$f.c $f 2>/dev/null | grep -E '^LOCK:'; done
rm -rf spl; python3 lane/tools/ownbss_splice.py spl/src/rom lane/ownbss func_80016E68 func_80016EE0 func_80016F80 >/dev/null
sh lane/tools/tusplice.sh lib_16320 | head -1
sh lane/tools/linkproof.sh
