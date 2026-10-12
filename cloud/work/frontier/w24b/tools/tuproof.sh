#!/bin/sh
# w24b (builder, scratch root): build a ROM TU file through asm-processor like the Makefile ROM-TU rule with
# IDO 5.3 (or $CC) at TU CFLAGS ($CF; default -O2 -g0 ROM-TU flags), link .text at TEXT_VRAM with every
# undefined symbol at its address (symbol_addrs / undefined_*_auto), compare the linked .text with
# baserom.us.z64[ROM_OFF .. +len].   usage: sh lane/tools/tuproof.sh TUFILE TEXT_VRAM ROM_OFF [RODATA_VRAM]
set -eu
CC=${CC:-$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido/cc}
CF=${CF:-"-G 0 -mips2 -O2 -non_shared -Iinclude -Iinclude/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -Isrc/rom"}
F=$1; TV=$2; RO=$3; RV=${4:-}; b=$(basename $F .c); d=lane/tuproof_$b; rm -rf $d; mkdir -p $d
python3 tools/asm-processor/build.py $CC -- mips-linux-gnu-as -march=vr4300 -mabi=32 -Iinclude -- -c $CF -o $d/$b.o $F > $d/build.log 2>&1 || { echo "BUILD FAIL"; tail -5 $d/build.log; exit 1; }
{ echo "SECTIONS {"; echo "  .text $TV : { *(.text) }"; echo "  .data : { *(.data) }"; echo "  .rodata $RV : SUBALIGN(4) { *(.rodata) }"; echo "  .bss (NOLOAD) : { *(.bss) }";
  echo "  /DISCARD/ : { *(.mdebug) *(.gptab.*) *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.comment) *(.note*) }"; echo "}"
  mips-linux-gnu-nm -u $d/$b.o | awk '{print $2}' | while read s; do
    a=$(grep -h "^ *$s *= *0x" symbol_addrs.us.txt undefined_syms_auto.us.txt undefined_funcs_auto.us.txt | head -1 | sed 's/.*= *\(0x[0-9A-Fa-f]*\).*/\1/')
    if [ -z "$a" ]; then
      if mips-linux-gnu-objdump -r $d/$b.o | awk '{print $3}' | grep -qx "$s"; then echo "UNRESOLVED $s" >&2; exit 1; fi
      a=0; fi   # declared-only extern (-g1 keeps it in the symbol table), no relocation uses it
    echo "$s = $a;"; done; } > $d/link.ld
mips-linux-gnu-ld -T $d/link.ld -o $d/$b.elf $d/$b.o
mips-linux-gnu-objcopy -O binary -j .text $d/$b.elf $d/text.bin
N=$(stat -c %s $d/text.bin)
dd if=baserom.us.z64 of=$d/rom.bin bs=1 skip=$(($RO)) count=$N 2>/dev/null
echo "$b: linked .text $N B sha1 $(sha1sum < $d/text.bin | cut -c1-40); rom $RO+$N sha1 $(sha1sum < $d/rom.bin | cut -c1-40)"
for s in .data .rodata .bss; do mips-linux-gnu-readelf -SW $d/$b.o | awk -v s=$s '$2==s{print "  " s " size 0x" $6}'; done
cmp $d/text.bin $d/rom.bin && echo "$b: linked .text == baserom: IDENTICAL"
