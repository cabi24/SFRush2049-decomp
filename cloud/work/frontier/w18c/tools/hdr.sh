#!/bin/sh
# w18c (builder, scratch root): include_patch/sequence_context.h is layout-neutral: every ROM TU that includes
# it built with IDO 7.1 against the original header (lane/include_orig/) and the patched one -> identical dumps.
set -u
CC=tools/ido-static-recomp/build71/out/cc
CF="-G 0 -mips2 -O2 -non_shared -Iinclude -Iinclude/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -Isrc/rom"
mkdir -p tuout
for v in orig patch; do
  case $v in orig) cp lane/include_orig/sequence_context.h include/;; patch) cp lane/include_patch/sequence_context.h include/;; esac
  for t in $(grep -l sequence_context.h src/rom/*.c | xargs -n1 basename | sed 's/\.c$//'); do
    python3 tools/asm-processor/build.py $CC -- mips-linux-gnu-as -march=vr4300 -mabi=32 -Iinclude -- -c $CF -o tuout/$t.h$v.o src/rom/$t.c > tuout/$t.h$v.log 2>&1 || echo "$t $v BUILD FAIL"
    { mips-linux-gnu-objdump -dr tuout/$t.h$v.o | sed -n '/^Disassembly/,$p'; for s in .data .rodata .bss; do mips-linux-gnu-objdump -s -j $s tuout/$t.h$v.o 2>/dev/null | sed -n '/^Contents/,$p'; done; } > tuout/$t.h$v.dump
  done
done
for t in $(grep -l sequence_context.h src/rom/*.c | xargs -n1 basename | sed 's/\.c$//'); do
  cmp -s tuout/$t.horig.dump tuout/$t.hpatch.dump && echo "$t: header patch IDENTICAL ($(wc -l < tuout/$t.horig.dump) dump lines)" || echo "$t: header patch DIFFERENT"; done
