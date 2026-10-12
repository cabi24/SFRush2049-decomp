#!/bin/sh
# w26b full proof (builder, scratch root ~/rush2049/scratch/frontier/w27s; lane dir synced to ./lane), w21b recipe.
# Pi side first (read-only): tools/resplit_target.py lane/reloc_addrs_w26b.txt OUT func_8001A658 -> tgt/<fn>_rs.o.
# 1. IDO 7.1 context rows for lane/sources (-> lane/ctx71_O2.jsonl)
# 2. LOCK (lock add verify_entry, toolkit 4586d230) vs the current target and vs the re-split target; TU-context check
# 3. all sources spliced (promote_batch --prefix-dedup replica) into the current TU == the TU whose GLOBAL_ASM is
#    re-split with reloc_addrs_w26b.txt (resplit_asm.py on a temporary copy of this scratch's asm/, restored after)
# TU edit: tools/tu_edits.py (ChannelState typedef + D_8004F300 extern hoisted above the slot), proven alone.
set -u
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
S=lane/sources
r=""; for f in $(ls $S | sed "s/\.c$//"); do r="$r $f=$S/$f.c"; done
echo "== context rows"
python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 lane/ctx71_O2.jsonl $r
echo "== LOCK"
for f in $(ls $S | sed 's/\.c$//'); do
  printf '%s vs current target: ' $f; TK=$TK python3 lane/tools/check.py $S/$f.c $f $f 2>/dev/null | grep '^LOCK:'
  [ -f tgt/${f}_rs.o ] && { printf '%s vs re-split target: ' $f; TK=$TK python3 lane/tools/check.py $S/$f.c $f ${f}_rs 2>/dev/null | grep '^LOCK:'; }
  rm -rf ed; python3 lane/tools/tu_edits.py ed/src/rom >/dev/null
  tu=$(grep -l "GLOBAL_ASM(\"asm/us/nonmatchings/rom/[^/]*/$f.s\")" ed/src/rom/*.c | head -1)
  printf '%s TU context (re-split target): ' $f; TK=$TK python3 lane/tools/check.py $S/$f.c $f ${f}_rs --tu $tu \
    --tu-flags "-G 0 -mips2 -O2 -non_shared -I include -I include/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -I src/rom" \
    --ctx lane/ctx71_O2.jsonl 2>/dev/null | grep -E '^(TU:|RESULT)' | tr '\n' ' '; echo
done
echo "== TU edit alone (lane/tools/tu_edits.py) vs current TU"
rm -rf ed spl tusrc; python3 lane/tools/tu_edits.py ed/src/rom >/dev/null
mkdir -p spl/src/rom; cp ed/src/rom/*.c spl/src/rom/
sh lane/tools/tusplice.sh $(ls ed/src/rom | sed 's/\.c$//')
echo "== all sources spliced into the edited TU, vs the current TU with re-split GLOBAL_ASM"
rm -rf spl; mkdir -p spl/src/rom tusrc; cp src/rom/*.c tusrc/; cp ed/src/rom/*.c tusrc/
TUSRC=tusrc python3 lane/tools/splice.py lane/ctx71_O2.jsonl spl/src/rom $(ls $S | sed 's/\.c$//')
rm -rf asm.orig; cp -r asm asm.orig
python3 lane/tools/resplit_asm.py lane/reloc_addrs_w26b.txt asm/us/nonmatchings/rom/lib_1a660/func_8001A658.s
sh lane/tools/tusplice.sh $(ls spl/src/rom | sed 's/\.c$//')
rm -rf asm; mv asm.orig asm
