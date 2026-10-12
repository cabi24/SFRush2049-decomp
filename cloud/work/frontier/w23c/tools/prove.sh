#!/bin/sh
# w23c full proof (builder, scratch root ~/rush2049/scratch/frontier/w23c; lane dir synced to ./lane).
# Scratch prerequisites: tgt/<fn>.o (w17k fetch_targets.py), rom_owned_data.json = the repo's + lane/data/owned_func_8001E0E0.json
# (scratch copy only), baserom.us.z64 + symbol files, tools/ido-static-recomp/build71/out -> the 4586d230 toolkit's ido/.
# 1. IDO 7.1 context rows for lane/sources (-> lane/ctx71_O2.jsonl)
# 2. LOCK (lock add verify_entry, toolkit 4586d230; flags from each source's /* flags: */ line)
# 3. sources spliced (promote_batch --prefix-dedup replica) into their TUs vs the current TUs
# 4. lib_1ece0 (owned .rodata): link the spliced and original TUs, .text == baserom; spliced .rodata at 0x8002D910 == baserom
set -u
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
S=lane/sources
r=""; for f in $(ls $S | sed "s/\.c$//"); do r="$r $f=$S/$f.c"; done
echo "== context rows"
python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 lane/ctx71_O2.jsonl $r
python3 -c "
import json
for l in open('lane/ctx71_O2.jsonl'):
    r=json.loads(l); print(r['function'], r['status'], r.get('compiler'), r.get('reason',''), [ (o['section'], o.get('size')) for o in r.get('owned',[])])"
echo "== LOCK"
for f in $(ls $S | sed 's/\.c$//'); do
  printf '%s: ' $f; TK=$TK python3 lane/tools/check.py $S/$f.c $f $f 2>/dev/null | grep '^LOCK:'
done
echo "== sources spliced into their TUs vs current TUs"
rm -rf spl; mkdir -p spl/src/rom
python3 lane/tools/splice.py lane/ctx71_O2.jsonl spl/src/rom $(ls $S | sed 's/\.c$//') >/dev/null
sh lane/tools/tusplice.sh $(ls spl/src/rom | sed 's/\.c$//')
echo "== lib_1ece0 linked (owned .rodata at 0x8002D910)"
sh lane/tools/linkproof.sh lib_1ece0 0x8001E0E0 0x8002DF00 0x8002D910
d=lane/linkproof_lib_1ece0
mips-linux-gnu-objcopy -O binary -j .rodata $d/spl.elf $d/spl.rodata.bin
head -c 20 $d/spl.rodata.bin > $d/spl.rodata20.bin; dd if=baserom.us.z64 of=$d/rom.rodata20.bin bs=1 skip=$((0x2E510)) count=20 2>/dev/null
echo "spliced .rodata[0..0x14] $(od -An -tx1 $d/spl.rodata20.bin | tr -d ' \n'); tail $(tail -c +21 $d/spl.rodata.bin | od -An -tx1 | tr -d ' \n')"
cmp $d/spl.rodata20.bin $d/rom.rodata20.bin && echo "spliced .rodata 0x8002D910..+0x14 == baserom 0x2E510: IDENTICAL"
echo "== libultra (IDO 5.3, lib_10d60): osPfsIsPlug, __osPfsRequestData (no inventory target: compared with the retail .s)"
T53=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5
L=lane/libultra
CONVEYOR_TOOLKIT=$T53 python3 cloud/work/boot_tail_promotion/context_check.py lane/ctx53_O2.jsonl osPfsIsPlug=$L/osPfsIsPlug.c __osPfsRequestData=$L/__osPfsRequestData.c
mkdir -p lt
for f in osPfsIsPlug __osPfsRequestData; do
  python3 -c "
import sys; sys.path[:0]=['.']
from tools.conveyor.seeds import context as c
open('lt/$f.r.c','w').write(c.reduced_tu(open('$L/$f.c').read(),'$f'))"
  $T53/ido/cc -c -g0 -O2 -mips2 -G 0 -non_shared -I include -I include/PR -I $T53/shim -o lt/$f.o lt/$f.r.c
  python3 lane/tools/asmcmp.py lt/$f.o $f asm/us/nonmatchings/rom/lib_10d60/$f.s | tail -1
done
rm -rf spl; mkdir -p spl/src/rom
SRCDIR=$L python3 lane/tools/splice.py lane/ctx53_O2.jsonl spl/src/rom osPfsIsPlug __osPfsRequestData >/dev/null
CC=$T53/ido/cc sh lane/tools/tusplice.sh lib_10d60
echo "== ownbss (lib_16320 .bss block 0x80042630/0x80; scratch rom_owned_data.json = lane/data/owned_func_80016E68_bss_grown.json replacing the func_80016E68 row)"
B=lane/ownbss
CONVEYOR_TOOLKIT=$TK python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 lane/ctx71_ownbss.jsonl func_80016C20=$B/func_80016C20.c func_80016CF0=$B/func_80016CF0.c func_80017040=$B/func_80017040.c
for f in func_80016C20 func_80016CF0 func_80017040; do printf '%s: ' $f; TK=$TK python3 lane/tools/check.py $B/$f.c $f $f 2>/dev/null | grep '^LOCK:'; done
rm -rf spl; mkdir -p spl/src/rom tusrc; cp src/rom/*.c tusrc/
TUSRC=tusrc python3 lane/tools/splice.py lane/ctx71_O2.jsonl tusrc func_8001605C >/dev/null
SRCDIR=$B TUSRC=tusrc python3 lane/tools/splice.py lane/ctx71_ownbss.jsonl spl/src/rom func_80016C20 func_80016CF0 func_80017040 >/dev/null
sh lane/tools/tusplice.sh lib_16320 | head -3
mips-linux-gnu-readelf -S tuout/lib_16320.spl.o | grep -E ' \.bss|\.text ' 
sh lane/tools/linkproof.sh lib_16320 0x80015720 0x80042630 2>&1 | grep -E 'spl:|rom:|spliced == baserom'
echo "== relocname func_80015A0C/func_80015D68 (reloc_addrs_w23c.txt; re-split targets tgt/<fn>_rs.o from resplit_target.py on the Pi)"
R=lane/relocname
CONVEYOR_TOOLKIT=$TK python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 lane/ctx71_relocname.jsonl func_80015A0C=$R/func_80015A0C.c func_80015D68=$R/func_80015D68.c
for f in func_80015A0C func_80015D68; do
  printf '%s vs current target: ' $f; TK=$TK python3 lane/tools/check.py $R/$f.c $f $f 2>/dev/null | grep '^LOCK:'
  printf '%s vs re-split target: ' $f; TK=$TK python3 lane/tools/check.py $R/$f.c $f ${f}_rs 2>/dev/null | grep '^LOCK:'
done
echo "-- TU edit alone (tools/tu_edits.py: two declaration hoists in lib_16320)"
rm -rf ed spl; python3 lane/tools/tu_edits.py ed/src/rom >/dev/null; mkdir -p spl/src/rom; cp ed/src/rom/lib_16320.c spl/src/rom/
sh lane/tools/tusplice.sh lib_16320 | head -1
echo "-- lib_16320 complete: edited TU + func_8001605C + ownbss trio + relocname pair, linked (.bss at 0x80042630) vs baserom"
rm -rf spl tusrc; mkdir -p spl/src/rom tusrc; cp src/rom/*.c tusrc/; cp ed/src/rom/lib_16320.c tusrc/
TUSRC=tusrc python3 lane/tools/splice.py lane/ctx71_O2.jsonl tusrc func_8001605C >/dev/null
SRCDIR=$B TUSRC=tusrc python3 lane/tools/splice.py lane/ctx71_ownbss.jsonl tusrc func_80016C20 func_80016CF0 func_80017040 >/dev/null
SRCDIR=$R TUSRC=tusrc python3 lane/tools/splice.py lane/ctx71_relocname.jsonl spl/src/rom func_80015A0C func_80015D68 >/dev/null
grep -c GLOBAL_ASM spl/src/rom/lib_16320.c
sh lane/tools/tusplice.sh lib_16320 | head -1
sh lane/tools/linkproof.sh lib_16320 0x80015720 0x80042630 2>&1 | grep -E 'spl:|rom:|spliced == baserom'
