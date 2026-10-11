#!/bin/sh
# w16g dry-run replay (builder, scratch root ~/rush2049/scratch/frontier/w16g; lane dir synced to ./lane,
# cloud/work/frontier/w16g -> lane, include/ src/rom/ tools/conveyor/ from the Pi tree, tgt/*.o = conveyor
# BlobStore[n64_target.target_o_sha], tools/ido-static-recomp/build/out -> toolkit d4c39cbc.../ido).
set -u
S=cloud/work/frontier/w16g/sources
O2TU="-G 0 -mips2 -O2 -non_shared -I include -I include/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -I src/rom"
O1TU="-G 0 -mips2 -O1 -non_shared -I include -I include/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -I src/rom"
LLTU="-G 0 -mips3 -32 -O1 -non_shared -I include -I include/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -I src/rom"
O2F="__osPfsRWInode __osContRamWrite __osEPiRawWriteIo __osEPiRawReadIo __osContAddressCrc __osContDataCrc osMotorStop __osSiDeviceBusy func_800103A0 func_80014650 func_8002517C osAiSetNextBuffer"
O1F="__osResetGlobalIntMask __osSetGlobalIntMask osYieldThread osDestroyThread"
LLF="__lshrdi3 __umoddi3 __udivdi3 __ashldi3 __umoddi3_alt __divdi3 __muldi3 __qdivrem __moddi3 __ashrdi3 __fixdfdi __fixsfdi __fixunsdfdi __fixunssfdi __floatdidf __floatdisf __floatundidf __floatundisf"
a() { r=""; for f in "$@"; do r="$r $f=$S/$f.c"; done; echo $r; }
python3 cloud/work/boot_tail_promotion/context_check.py lane/ctx/ctx_O2.jsonl $(a $O2F)
CTX_OPT=-O1 python3 lane/ctx.py lane/ctx/ctx_O1.jsonl $(a $O1F)
CTX_ISA="-mips3 -32" CTX_OPT=-O1 python3 lane/ctx.py lane/ctx/ctx_ll.jsonl $(a $LLF)
# func_80014650: lib_11640.c already declares D_80038368 (as MessageQueue) before the slot; drop that line.
python3 -c "
import json
rows=[json.loads(l) for l in open('lane/ctx/ctx_O2.jsonl')]
for r in rows:
    if r['function']=='func_80014650':
        r['preamble']=[s for s in r['preamble'] if 'D_80038368' not in s]; r['note']='D_80038368 declared by lib_11640.c before the slot (extern MessageQueue D_80038368)'
open('lane/ctx/ctx_O2.jsonl','w').write(''.join(json.dumps(r)+'\n' for r in rows))"
tgt() { case $1 in __osEPiRawWriteIo) echo osEPiRawStartDma;; __osEPiRawReadIo) echo osEPiRawReadIo;; __osContDataCrc) echo __osPfsDataChecksum;;
  __osSiDeviceBusy) echo __osPiDeviceBusy;; __osResetGlobalIntMask) echo osEPiRawWriteIo;; __osSetGlobalIntMask) echo __osPiGetCmdQueue;;
  osDestroyThread) echo __osEnqueueAndYield;; *) echo $1;; esac; }
tu() { case $1 in __osPfsRWInode) echo "--tu src/rom/lib_f700.c";; osMotorStop) echo "--tu src/rom/lib_ab20.c";; osAiSetNextBuffer) echo "--tu src/rom/lib_ca70.c";;
  func_80014650) echo "--tu src/rom/lib_11640.c";; func_8002517C) echo "--tu src/rom/lib_25bb0.c";; *) echo "";; esac; }
for f in $O2F; do python3 lane/check.py lane/sources/$f.c $f $(tgt $f) $(tu $f) --ctx lane/ctx/ctx_O2.jsonl --tu-flags "$O2TU" $( [ $f = osAiSetNextBuffer ] && echo "--additions lane/ctx/osAiSetNextBuffer.h" ); done
for f in $O1F; do python3 lane/check.py lane/sources/$f.c $f $(tgt $f) --ctx lane/ctx/ctx_O1.jsonl --tu-flags "$O1TU"; done
for f in $LLF; do python3 lane/check.py lane/sources/$f.c $f $f --ctx lane/ctx/ctx_ll.jsonl --tu-flags "$LLTU"; done
