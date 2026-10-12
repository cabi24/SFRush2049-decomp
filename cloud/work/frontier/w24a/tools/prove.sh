#!/bin/sh
# w24a full proof (builder, scratch root ~/rush2049/scratch/frontier/w24a; lane dir synced to ./lane).
# Scratch prerequisites: tgt/<fn>.o (w17k fetch_targets.py), the CONVERTED TU from lane/conversion (src/rom/lib_26f60.c +
# asm/us/nonmatchings/rom/lib_26f60/*.s, as `layout convert 0x26f60` writes them), tools/asm-processor,
# tools/ido-static-recomp/build71/out -> the 4586d230 toolkit's ido/, baserom.us.z64.
# 1. IDO 7.1 context rows (-> lane/ctx71_O2.jsonl)  2. LOCK (lock add verify_entry) + TU (ctx preamble, real TU prefix)
# 3. all four spliced (promote_batch --prefix-dedup replica) vs the all-passthrough TU   4. spliced .text vs baserom
set -u
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
S=lane/sources
FNS="func_80026360 func_80026558 func_80026640 func_800268D0"
echo "== context rows"
r=""; for f in $FNS; do r="$r $f=$S/$f.c"; done
rm -f lane/ctx71_O2.jsonl
python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 lane/ctx71_O2.jsonl $r
python3 -c "
import json
for l in open('lane/ctx71_O2.jsonl'):
    r=json.loads(l); print(r['function'], r['status'], r.get('compiler'), r.get('reason',''), len(r['preamble']), 'dropped', r['dropped'])"
echo "== LOCK + TU"
O2TU="-G 0 -mips2 -O2 -non_shared -I include -I include/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -I src/rom"
for f in $FNS; do
  printf '%s: ' $f; TK=$TK python3 lane/tools/check.py $S/$f.c $f $f --ctx lane/ctx71_O2.jsonl --tu src/rom/lib_26f60.c --tu-flags "$O2TU" 2>/dev/null | grep -E '^(LOCK|TU|RESULT)' | tr '\n' ' '; echo
done
echo "== all four spliced into lib_26f60 vs the all-passthrough TU (IDO 7.1, Makefile ROM-TU flags, asm-processor)"
rm -rf spl; mkdir -p spl/src/rom
python3 lane/tools/splice.py lane/ctx71_O2.jsonl spl/src/rom $FNS >/dev/null
grep -c GLOBAL_ASM spl/src/rom/lib_26f60.c
sh lane/tools/tusplice.sh lib_26f60
echo "== spliced lib_26f60 .text linked at 0x80026360 vs baserom 0x26F60+0x1470"
d=lane/linkproof; mkdir -p $d
printf 'SECTIONS { . = 0x80026360; .text : { *(.text) } /DISCARD/ : { *(*) } }\nfunc_800277D0 = 0x800277D0;\nfunc_800278C4 = 0x800278C4;\nfunc_80027B44 = 0x80027B44;\nbzero = 0x80008590;\n' > $d/l.ld
for k in orig spl; do
  mips-linux-gnu-ld -o $d/$k.elf -T $d/l.ld tuout/lib_26f60.$k.o && mips-linux-gnu-objcopy -O binary -j .text $d/$k.elf $d/$k.bin
  printf '%s: %s bytes sha1 %s\n' $k $(stat -c %s $d/$k.bin) $(sha1sum < $d/$k.bin | cut -c1-40)
done
dd if=baserom.us.z64 of=$d/rom.bin bs=1 skip=$((0x26F60)) count=$((0x1470)) 2>/dev/null
printf 'rom: %s bytes sha1 %s\n' $(stat -c %s $d/rom.bin) $(sha1sum < $d/rom.bin | cut -c1-40)
cmp $d/spl.bin $d/rom.bin && echo "spliced lib_26f60 .text == baserom 0x26F60..0x283D0: IDENTICAL"
