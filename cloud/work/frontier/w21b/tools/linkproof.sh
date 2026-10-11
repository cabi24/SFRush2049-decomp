#!/bin/sh
# w21b (from w20b; builder, scratch root): linked proof for a TU with function-static .bss.
# usage: sh lane/tools/linkproof.sh TU TEXT_VRAM BSS_VRAM [RODATA_VRAM]   (ROM offset = TU hex suffix; length = linked .text size)
# The TU .bss is placed at BSS_VRAM exactly (SUBALIGN(4) overrides its 16-byte input alignment: retail has
# other .bss before it in an aligned block that this proof does not reconstruct).
# Links tuout/lib_16320.{orig,spl}.o (from tusplice.sh) with GNU ld: .text at the TU's vram 0x80015720,
# the TU's .bss at 0x80042670 (the proposed owned storage block), every undefined symbol at its address
# (NAME_XXXXXXXX -> 0xXXXXXXXX, else symbol_addrs/undefined_*_auto.us.txt). Compares the linked .text
# bytes of both with each other and with baserom.us.z64[0x16320 .. +0x1aa0] (ROM 0x16320 = vram 0x80015720).
set -eu
TU=$1; TV=$2; BV=$3; RV=${4:-}
d=lane/linkproof_$TU; rm -rf $d; mkdir -p $d
for k in orig spl; do
  o=tuout/$TU.$k.o
  { echo "SECTIONS {"; echo "  .text $TV : { *(.text) }"; echo "  .bss $BV (NOLOAD) : SUBALIGN(4) { *(.bss) }"; [ -z "$RV" ] || echo "  .rodata $RV : { *(.rodata) }";
    echo "  /DISCARD/ : { *(.mdebug) *(.gptab.*) *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.comment) *(.note*) }"; echo "}"
    mips-linux-gnu-nm -u $o | awk '{print $2}' | while read s; do
      a=$(echo "$s" | sed -n 's/^.*_\([0-9A-Fa-f]\{8\}\)$/0x\1/p')
      [ -n "$a" ] || a=$(grep -h "^ *$s *= *0x" symbol_addrs.us.txt undefined_syms_auto.us.txt undefined_funcs_auto.us.txt | head -1 | sed 's/.*= *\(0x[0-9A-Fa-f]*\).*/\1/')
      [ -n "$a" ] || { echo "UNRESOLVED $s" >&2; exit 1; }
      echo "$s = $a;"; done; } > $d/$k.ld
  mips-linux-gnu-ld -T $d/$k.ld -o $d/$k.elf $o
  mips-linux-gnu-objcopy -O binary -j .text $d/$k.elf $d/$k.text.bin
  echo "$k: .text $(stat -c %s $d/$k.text.bin) B, sha1 $(sha1sum < $d/$k.text.bin | cut -c1-40); .bss $(mips-linux-gnu-readelf -S $d/$k.elf | grep -o '\.bss *NOBITS *[0-9a-f]* [0-9a-f]* [0-9a-f]*' || echo none)"
done
R=0x${TU#lib_}; N=$(stat -c %s $d/orig.text.bin)
dd if=baserom.us.z64 of=$d/rom.text.bin bs=1 skip=$((R)) count=$N 2>/dev/null
echo "rom:  .text $(stat -c %s $d/rom.text.bin) B, sha1 $(sha1sum < $d/rom.text.bin | cut -c1-40)"
cmp $d/orig.text.bin $d/spl.text.bin && echo "linked orig == linked spliced: IDENTICAL"
cmp $d/spl.text.bin $d/rom.text.bin && echo "linked spliced == baserom $R..+$N: IDENTICAL"
