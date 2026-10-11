#!/bin/sh
# w17g replay (builder, scratch root ~/rush2049/scratch/frontier/w17g; lane dir synced to ./lane; include/ src/rom/
# asm/us/ tools/conveyor/ tools/cloud/ from the Pi tree; tgt/*.o = conveyor BlobStore[n64_target.target_o_sha];
# tools/ido-static-recomp/build/out -> toolkit d4c39cbc.../ido). Uses w16g check.py unchanged (lane/check.py).
set -u
S=lane/sources
O2TU="-G 0 -mips2 -O2 -non_shared -I include -I include/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -I src/rom"
O2F="${O2F:-func_80014CF4 func_80014D14 func_8001E0D4 func_80014CFC func_80014D08 func_80014BD8 func_80014BF8}"
a() { r=""; for f in "$@"; do r="$r $f=$S/$f.c"; done; echo $r; }
python3 cloud/work/boot_tail_promotion/context_check.py lane/ctx/ctx_O2.jsonl $(a $O2F)
tu() { s=$(grep -l "GLOBAL_ASM(\"asm/us/nonmatchings/rom/[^/]*/$1.s\")" src/rom/*.c | head -1); [ -n "$s" ] && echo "--tu $s"; }
for f in $O2F; do python3 lane/check.py $S/$f.c $f $f $(tu $f) --ctx lane/ctx/ctx_O2.jsonl --tu-flags "$O2TU"; done
