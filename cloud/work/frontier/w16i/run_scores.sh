#!/bin/sh
# w16i: lock-style + TU scores (w16g check.py) on the builder scratch root ~/rush2049/scratch/frontier/w16i.
# tgt/<id>.o = the target objects `lock add --target <id>` scores against: for the 8 renamed boot functions
# the objects targets_rename registered in the scratch COPY of conveyor.db; for libgcc the existing rows
# (collect_targets.py). Writes nothing outside this scratch root.
set -u
L=cloud/work/frontier/w16g
S=$L/sources
O2TU="-G 0 -mips2 -O2 -non_shared -I include -I include/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -I src/rom"
O1TU="-G 0 -mips2 -O1 -non_shared -I include -I include/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -I src/rom"
LLTU="-G 0 -mips3 -32 -O1 -non_shared -I include -I include/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -I src/rom"
echo "### renamed boot functions: lock add --target <current name> (new inventory rows)"
for f in __osEPiRawWriteIo __osEPiRawReadIo __osContDataCrc __osSiDeviceBusy; do
  python3 $L/check.py $S/$f.c $f $f --ctx $L/ctx/ctx_O2.jsonl --tu-flags "$O2TU"; done
for f in __osResetGlobalIntMask __osSetGlobalIntMask osDestroyThread osYieldThread; do
  python3 $L/check.py $S/$f.c $f $f --ctx $L/ctx/ctx_O1.jsonl --tu-flags "$O1TU"; done
echo "### libgcc (ll.c / llcvt.c) at -g0 -O1 -mips3 -32"
for f in __lshrdi3 __umoddi3 __udivdi3 __ashldi3 __umoddi3_alt __divdi3 __muldi3 __qdivrem __moddi3 __ashrdi3 \
         __fixdfdi __fixsfdi __floatdidf __floatdisf __floatundidf __floatundisf; do
  python3 $L/check.py $S/$f.c $f $f --ctx $L/ctx/ctx_ll.jsonl --tu-flags "$LLTU"; done
