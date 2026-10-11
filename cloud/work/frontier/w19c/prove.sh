#!/bin/sh
# w19c builder proof, run from the scratch repo root (~/rush2049/scratch/frontier/w19c/repo = this branch's
# tree, IDO 5.3 at tools/ido-static-recomp/build/out -> toolkit 796ae99a.../ido, tgt/*.o = conveyor target
# objects: osAiSetNextBuffer = live BlobStore[n64_target.target_o_sha]; __fixuns*.o = the reloc-aware objects
# targets_rename registers (DB copy, w19c/apply_copy.log); __fixuns*.old.o = the live raw-word rows).
# 1. context rows with the committed context_check.py (owned data, --flags);
# 2. lock-style scores (w16g check.py = lock add's verify_entry compile + score; TU = real TU before the slot).
set -u
L=cloud/work/frontier/w19c
S=cloud/work/frontier/w16g/sources
LL="-g0 -O1 -mips3 -32 -G 0 -non_shared"
O2TU="-G 0 -mips2 -O2 -non_shared -I include -I include/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -I src/rom"
LLTU="-G 0 -mips3 -32 -O1 -non_shared -I include -I include/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -I src/rom"
mkdir -p $L/ctx
python3 cloud/work/boot_tail_promotion/context_check.py $L/ctx/ctx_ai.jsonl osAiSetNextBuffer=$S/osAiSetNextBuffer.c
python3 cloud/work/boot_tail_promotion/context_check.py --flags "$LL" $L/ctx/ctx_fixuns.jsonl \
  __fixunsdfdi=$S/__fixunsdfdi.c __fixunssfdi=$S/__fixunssfdi.c
cat $L/ctx/ctx_ai.jsonl $L/ctx/ctx_fixuns.jsonl > $L/ctx/ctx_w19c.jsonl
export TK=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5
python3 cloud/work/frontier/w16g/check.py $S/osAiSetNextBuffer.c osAiSetNextBuffer osAiSetNextBuffer \
  --tu src/rom/lib_ca70.c --ctx $L/ctx/ctx_w19c.jsonl --tu-flags "$O2TU"
for f in __fixunsdfdi __fixunssfdi; do
  python3 cloud/work/frontier/w16g/check.py $S/$f.c $f $f --tu src/rom/lib_a5f0.c --ctx $L/ctx/ctx_w19c.jsonl --tu-flags "$LLTU"
  echo "-- against the current (raw-word) inventory object:"
  python3 cloud/work/frontier/w16g/check.py $S/$f.c $f $f.old --tu src/rom/lib_a5f0.c --ctx $L/ctx/ctx_w19c.jsonl --tu-flags "$LLTU"
done
