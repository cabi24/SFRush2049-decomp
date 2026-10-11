#!/bin/sh
# w17g whole-TU compiler-switch proof (builder, scratch root). For each ROM TU given, build it exactly like the
# Makefile ROM-TU rule (asm-processor build.py, default -O2 CFLAGS) twice: with the pinned IDO 5.3
# (tools/ido-static-recomp/build/out = toolkit d4c39cbc) and with IDO 7.1 (tk71/ido), then compare
# `objdump -dr` (.text words + relocations) and `objdump -s` of every other section. 5.3 is what master links
# (ROM SHA-1 gated), so IDENTICAL means the TU can switch compiler without changing a byte.
# usage: sh tu71.sh SRCDIR lib_X [lib_Y ...]   (SRCDIR holds the src/rom/*.c to build; default src/rom)
set -u
SD=$1; shift
CF="-G 0 -mips2 -O2 -non_shared -Iinclude -Iinclude/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -Isrc/rom"
mkdir -p tuout
for t in "$@"; do
  for c in 53 71; do
    case $c in 53) CC=tools/ido-static-recomp/build/out/cc;; 71) CC=tk71/ido/cc;; esac
    python3 tools/asm-processor/build.py $CC -- mips-linux-gnu-as -march=vr4300 -mabi=32 -Iinclude -- -c $CF \
      -o tuout/$t.$c.o $SD/$t.c > tuout/$t.$c.log 2>&1 || { echo "$t: BUILD FAIL ($c)"; tail -3 tuout/$t.$c.log; continue 2; }
    { mips-linux-gnu-objdump -dr -j .text tuout/$t.$c.o | sed -n '/^Disassembly/,$p' | sed 's/^ *[0-9a-f]*:\t//' | cut -f1,2;
      for s in .data .rodata .bss; do mips-linux-gnu-objdump -s -j $s tuout/$t.$c.o 2>/dev/null | sed -n '/^Contents/,$p'; done; } > tuout/$t.$c.dump
  done
  n=$(grep -c . tuout/$t.53.dump)
  if cmp -s tuout/$t.53.dump tuout/$t.71.dump; then echo "$t: IDENTICAL ($n dump lines)"; else
    echo "$t: DIFFERENT ($(diff tuout/$t.53.dump tuout/$t.71.dump | grep -c '^<') of $n lines)"; fi
done
