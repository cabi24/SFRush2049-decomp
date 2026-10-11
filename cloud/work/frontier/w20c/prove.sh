#!/bin/sh
# w20c builder proof, run from the scratch repo root (~/rush2049/scratch/frontier/w20c/repo = branch w20c's tree,
# IDO 7.1 at tools/ido-static-recomp/build71/out; tgt/*.o = the live inventory target objects AFTER
# refresh_split_targets.py, fetched with cloud/work/frontier/w17k/fetch_targets.py).
# 1. context rows (context_check.py --compiler ido7.1; owned .rodata admitted for slot owners/co-owners);
# 2. lock-style scores (check.py = lock add's verify_entry compile + scoring.score/strict/reloc_blind +
#    objdump -dr compare; TU = the real TU text before the slot + the ctx preamble + body, ROM-TU flags).
set -u
L=cloud/work/frontier/w20c
S=$L/sources
O2TU="-G 0 -mips2 -O2 -non_shared -I include -I include/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -I src/rom"
export TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
r=""; for f in $(ls $S | sed 's/\.c$//'); do r="$r $f=$S/$f.c"; done
rm -f $L/ctx71_w20c.jsonl
python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 $L/ctx71_w20c.jsonl $r
for f in $(ls $S | sed 's/\.c$//'); do
  tu=$(grep -l "GLOBAL_ASM(\"asm/us/nonmatchings/rom/[^/]*/$f.s\")" src/rom/*.c | head -1)
  python3 $L/tools/check.py $S/$f.c $f $f --tu $tu --ctx $L/ctx71_w20c.jsonl --tu-flags "$O2TU" | grep -E "^(==|LOCK:|TU:|CTX|RESULT)"
done
