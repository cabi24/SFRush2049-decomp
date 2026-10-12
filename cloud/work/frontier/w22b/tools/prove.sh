#!/bin/sh
# w22b (builder, scratch root): proof for the lib_22300 owned-.bss/.rodata pair func_80022874 + func_80023E9C.
# 1 LOCK lines (check.py = lock add verify_entry, IDO 7.1) + row classes; 2 splice both into a copy of the CURRENT
# src/rom/lib_22300.c (prefix-dedup semantics), build the ROM TU with IDO 7.1 through asm-processor (Makefile
# CFLAGS); 3 link it with .text 0x80021700, .data 0x8002D460 (func_80022CD4's owned slot), .rodata 0x8002D950
# (owned slot), .bss 0x80056200 (proposed owned block) and compare .text with baserom, .rodata/.data with
# assets/us/data.bin, list the .bss symbols; 4 context_check rows (--compiler ido7.1).
set -u
CC=tools/ido-static-recomp/build71/out/cc
CF="-G 0 -mips2 -O2 -non_shared -Iinclude -Iinclude/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -Isrc/rom"
echo "== 1 LOCK"
for f in func_80022874 func_80023E9C; do echo "-- $f"; sh lane/tools/sc.sh lane/sources/$f.c $f 2>&1 | grep '^LOCK:'; sh lane/tools/rows.sh lane/sources/$f.c $f | grep -v '|'; done
echo "== 2 spliced TU build"
rm -rf splF; python3 lane/tools/ownbss_splice.py splF/src/rom lane/sources lib_22300 func_80022874 func_80023E9C
mkdir -p tuout
python3 tools/asm-processor/build.py $CC -- mips-linux-gnu-as -march=vr4300 -mabi=32 -Iinclude -- -c $CF -o tuout/lib_22300.spl.o splF/src/rom/lib_22300.c > tuout/spl.log 2>&1 && echo "spliced TU builds" || { echo BUILD FAIL; tail -5 tuout/spl.log; exit 1; }
python3 tools/asm-processor/build.py $CC -- mips-linux-gnu-as -march=vr4300 -mabi=32 -Iinclude -- -c $CF -o tuout/lib_22300.orig.o src/rom/lib_22300.c > tuout/orig.log 2>&1 && echo "current TU builds"
mips-linux-gnu-readelf -S tuout/lib_22300.spl.o | grep -E ' \.(text|data|rodata|bss) '
echo "== 3 linked proof"
sh lane/tools/linkproof22.sh lib_22300 0x80021700 0x80056200 0x8002D950 2>&1 | grep -v RWX
cd lane/linkproof_lib_22300 && for s in rodata data; do mips-linux-gnu-objcopy -O binary -j .$s spl.elf spl.$s.bin; done
python3 -c "
d=open('../../assets/us/data.bin','rb').read()
r=open('spl.rodata.bin','rb').read(); print('linked .rodata', hex(len(r)), 'B == data.bin[0x6180:] (0x8002D950):', r==d[0x6180:0x6180+len(r)])
x=open('spl.data.bin','rb').read(); print('linked .data', hex(len(x)), 'B == data.bin[0x5c90:] (0x8002D460):', x==d[0x5c90:0x5c90+len(x)])"
echo "linked .bss symbols:"; mips-linux-gnu-nm -n spl.elf | grep ' b '
cd ../..
echo "== 4 context rows"
python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 lane/ctx71_w22b.jsonl func_80022874=lane/sources/func_80022874.c func_80023E9C=lane/sources/func_80023E9C.c 2>&1 | tail -5
cat lane/ctx71_w22b.jsonl | python3 -c "
import json,sys
for l in sys.stdin: r=json.loads(l); print(r['function'], r.get('status'), r.get('compiler'), r.get('reason') or r.get('detail') or '')"
