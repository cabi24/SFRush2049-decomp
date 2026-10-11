#!/bin/sh
# w20b (builder, scratch root): linked proof for the lib_16320 static-.bss trio.
# Links tuout/lib_16320.{orig,spl}.o (from tusplice.sh) with GNU ld: .text at the TU's vram 0x80015720,
# the TU's .bss at 0x80042670 (the proposed owned storage block), every undefined symbol at its address
# (NAME_XXXXXXXX -> 0xXXXXXXXX, else symbol_addrs/undefined_*_auto.us.txt). Compares the linked .text
# bytes of both with each other and with baserom.us.z64[0x16320 .. +0x1aa0] (ROM 0x16320 = vram 0x80015720).
set -eu
d=lane/linkproof; rm -rf $d; mkdir -p $d
for k in orig spl; do
  o=tuout/lib_16320.$k.o
  { echo "SECTIONS {"; echo "  .text 0x80015720 : { *(.text) }"; echo "  .bss 0x80042670 (NOLOAD) : { *(.bss) }";
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
dd if=baserom.us.z64 of=$d/rom.text.bin bs=1 skip=$((0x16320)) count=$((0x1aa0)) 2>/dev/null
echo "rom:  .text $(stat -c %s $d/rom.text.bin) B, sha1 $(sha1sum < $d/rom.text.bin | cut -c1-40)"
cmp $d/orig.text.bin $d/spl.text.bin && echo "linked orig == linked spliced: IDENTICAL"
cmp $d/spl.text.bin $d/rom.text.bin && echo "linked spliced == baserom 0x16320..0x17DC0: IDENTICAL"
