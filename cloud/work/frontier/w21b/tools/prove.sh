#!/bin/sh
# w21b full proof (builder, scratch root ~/rush2049/scratch/frontier/w21b; lane dir synced to ./lane).
# Pi side first (read-only DB): fetch_targets.py tgt <fns> and tools/resplit_target.py lane/reloc_addrs_w21b.txt tgt
# func_8001671C func_80016998 (-> tgt/<fn>_rs.o), then sync tgt/ here.
# 1. IDO 7.1 context rows for lane/sources (-> lane/ctx71_O2.jsonl)
# 2. LOCK (lock add verify_entry, toolkit 4586d230) vs the current target, and vs the re-split target where one exists
# 3. TU edit alone (lib_16320 ResourceRange hoist) == original
# 4. all sources spliced (promote_batch --prefix-dedup replica) into the edited TUs == original TUs whose GLOBAL_ASM
#    is re-split with reloc_addrs_w21b.txt (resplit_asm.py on a temporary copy of this scratch's asm/, restored after)
# 5. ownbss func_80022874: LOCK line, manual splice into lib_22300, linked .text == baserom (static at 0x80056208)
set -u
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
S=lane/sources
r=""; for f in $(ls $S | sed "s/\.c$//"); do r="$r $f=$S/$f.c"; done
echo "== context rows (non-ok rows only)"
python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 lane/ctx71_O2.jsonl $r | grep -v " ok$"
echo "== LOCK"
for f in $(ls $S | sed 's/\.c$//'); do
  printf '%s vs current target: ' $f; TK=$TK python3 lane/tools/check.py $S/$f.c $f $f 2>/dev/null | grep '^LOCK:'
  [ -f tgt/${f}_rs.o ] && { printf '%s vs re-split target: ' $f; TK=$TK python3 lane/tools/check.py $S/$f.c $f ${f}_rs 2>/dev/null | grep '^LOCK:'; }
done
echo "== TU edit alone"
rm -rf ed spl tusrc; python3 lane/tools/tu_edits.py ed/src/rom >/dev/null
mkdir -p spl/src/rom; cp ed/src/rom/*.c spl/src/rom/
sh lane/tools/tusplice.sh $(ls ed/src/rom | sed 's/\.c$//')
echo "== all sources spliced into the edited TUs, vs original TUs with re-split GLOBAL_ASM"
mkdir -p tusrc; cp src/rom/*.c tusrc/; cp ed/src/rom/*.c tusrc/
rm -rf spl; mkdir -p spl/src/rom
TUSRC=tusrc python3 lane/tools/splice.py lane/ctx71_O2.jsonl spl/src/rom $(ls $S | sed 's/\.c$//') >/dev/null
rm -rf asm.orig; cp -r asm asm.orig
python3 lane/tools/resplit_asm.py lane/reloc_addrs_w21b.txt asm/us/nonmatchings/rom/lib_16320/func_8001671C.s asm/us/nonmatchings/rom/lib_16320/func_80016998.s
sh lane/tools/tusplice.sh $(ls spl/src/rom | sed 's/\.c$//')
rm -rf asm; mv asm.orig asm
echo "== ownbss func_80022874 (function-local static record; context_check refuses .bss, manual splice)"
printf 'func_80022874 '; TK=$TK python3 lane/tools/check.py lane/ownbss/func_80022874.c func_80022874 func_80022874 2>/dev/null | grep '^LOCK:'
rm -rf spl; python3 lane/tools/ownbss_splice.py spl/src/rom lane/ownbss lib_22300 func_80022874 >/dev/null
sh lane/tools/tusplice.sh lib_22300 | head -1
sh lane/tools/linkproof.sh lib_22300 0x80021700 0x80056208 0x8002D950
