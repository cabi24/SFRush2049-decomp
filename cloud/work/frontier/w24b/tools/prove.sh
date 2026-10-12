#!/bin/bash
# w24b full proof (builder, scratch root ~/rush2049/scratch/frontier/w24b; lane dir synced to ./lane).
# Prereqs: tgt/<fn>.o (w17k fetch_targets.py), baserom.us.z64, repo src/rom include tools symbol files synced.
# IDO 5.3 (toolkit 796ae99a) for every TU here. Output: lane/prove.log
set -u
T53=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5
FLG1="-g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul"
TUG1="-G 0 -mips2 -O1 -non_shared -Iinclude -Iinclude/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -Isrc/rom -g1"
S=lane/sources
mkdir -p w; cp src/rom/rom_tu.h w/orig_rom_tu.h   # master rom_tu.h (synced from the Pi); restored after each patch
lock() { printf '%s vs %s: ' $2 $3; TK=$T53 python3 lane/tools/check.py $1 $2 $3 2>&1 | grep '^LOCK:'; }

echo "== targets with stale pre-w22g callee names -> emulated refresh (objcopy --redefine-sym, names only)"
mips-linux-gnu-objcopy --redefine-sym osSetGlobalIntMask=osSetIntMask tgt/viTickStart.o tgt/viTickStart_rn.o
mips-linux-gnu-objcopy --redefine-sym osViSetMode=osViSwapBuffer --redefine-sym osJamMesg=osSendMesg tgt/__scHandleRetrace.o tgt/__scHandleRetrace_rn.o
mips-linux-gnu-objcopy --redefine-sym osInvalICache_full=osInvalICache --redefine-sym osJamMesg=osSendMesg tgt/game_init.o tgt/game_init_rn.o
echo "== 1. rmonPrintf (lib_92f0, -g0 -O2)"
lock $S/rmonPrintf.c rmonPrintf rmonPrintf
rm -f lane/ctx53_O2.jsonl; CONVEYOR_TOOLKIT=$T53 python3 cloud/work/boot_tail_promotion/context_check.py lane/ctx53_O2.jsonl rmonPrintf=$S/rmonPrintf.c
rm -rf w/p92; python3 lane/tools/splice.py lane/ctx53_O2.jsonl w/p92 rmonPrintf >/dev/null
sh lane/tools/tuproof.sh src/rom/lib_92f0.c 0x800086F0 0x92F0 | tail -1
sh lane/tools/tuproof.sh w/p92/lib_92f0.c 0x800086F0 0x92F0 | tail -1
echo "== 2. scheduler (lib_1050_sdk, -g1 -O1)"
lock $S/__scAppendList.c __scAppendList __scAppendList
lock $S/__scHandleRetrace.c __scHandleRetrace __scHandleRetrace
lock $S/__scHandleRetrace.c __scHandleRetrace __scHandleRetrace_rn
rm -f lane/ctx53_sched.jsonl; CONVEYOR_TOOLKIT=$T53 python3 cloud/work/boot_tail_promotion/context_check.py --flags "$FLG1" lane/ctx53_sched.jsonl __scAppendList=$S/__scAppendList.c __scHandleRetrace=$S/__scHandleRetrace.c
rm -rf w/sdk; python3 lane/tools/splice.py lane/ctx53_sched.jsonl w/sdk __scAppendList __scHandleRetrace >/dev/null
CF="$TUG1" sh lane/tools/tuproof.sh src/rom/lib_1050_sdk.c 0x80000450 0x1050 0x8002D4A4 | tail -1
CF="$TUG1" sh lane/tools/tuproof.sh w/sdk/lib_1050_sdk.c 0x80000450 0x1050 0x8002D4A4 | tail -1
echo "-- research probes (not deliverables): __scMain / __scExec with timestamps volatile + defined"
lock lane/sched/__scMain_probe.c __scMain __scMain
lock lane/sched/__scExec_probe.c __scExec __scExec
echo "== 3. game_init (lib_2cf0, -g1 -O1)"
lock $S/game_init.c game_init game_init
lock $S/game_init.c game_init game_init_rn
rm -f lane/ctx53_gi.jsonl; CONVEYOR_TOOLKIT=$T53 python3 cloud/work/boot_tail_promotion/context_check.py --flags "$FLG1" lane/ctx53_gi.jsonl game_init=$S/game_init.c
rm -rf w/gi; python3 lane/tools/splice.py lane/ctx53_gi.jsonl w/gi game_init >/dev/null
CF="$TUG1" sh lane/tools/tuproof.sh src/rom/lib_2cf0.c 0x800020F0 0x2CF0 | tail -1
CF="$TUG1" sh lane/tools/tuproof.sh w/gi/lib_2cf0.c 0x800020F0 0x2CF0 | tail -1
echo "== 4. vi* (segment 0x1f50 = lib_1050, -g1 -O1; needs lane/patch/rom_tu_volatile.diff)"
for f in viTickStart viEnableAccum viDisableAccum viUpdateTime viScheduleTick viAddTicks viGetTimeToDeadline; do lock $S/$f.c $f $f; done
lock $S/viTickStart.c viTickStart viTickStart_rn
patch -s src/rom/rom_tu.h lane/patch/rom_tu_volatile.diff && echo "rom_tu.h patched (scratch copy)"
r=""; for f in viTickStart viEnableAccum viDisableAccum viUpdateTime viScheduleTick viAddTicks viGetTimeToDeadline; do r="$r $f=$S/$f.c"; done
rm -f lane/ctx53_vi.jsonl; CONVEYOR_TOOLKIT=$T53 python3 cloud/work/boot_tail_promotion/context_check.py --flags "$FLG1" lane/ctx53_vi.jsonl $r
rm -rf w/vi; python3 lane/tools/vi1050_tu.py w/vi >/dev/null
echo "-- today's lib_1050.c at -O2 (control, unpatched header)"; cp w/orig_rom_tu.h src/rom/rom_tu.h
sh lane/tools/tuproof.sh src/rom/lib_1050.c 0x80001350 0x1F50 | tail -1
patch -s src/rom/rom_tu.h lane/patch/rom_tu_volatile.diff
echo "-- spliced lib_1050.c (7 vi bodies, viStub removed) at -g1 -O1, patched header"
CF="$TUG1" sh lane/tools/tuproof.sh w/vi/lib_1050.c 0x80001350 0x1F50 | tail -1
cp w/orig_rom_tu.h src/rom/rom_tu.h
echo "== context rows"; for j in lane/ctx53_O2.jsonl lane/ctx53_sched.jsonl lane/ctx53_gi.jsonl lane/ctx53_vi.jsonl; do
python3 -c "
import json,sys
for l in open('$j'):
    r=json.loads(l); print('$j', r['function'], r['status'], r.get('compiler'), r.get('flagset','(default)'), r.get('reason',''))"; done
