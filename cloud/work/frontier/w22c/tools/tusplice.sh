#!/bin/sh
# w18c spliced-TU proof (builder, scratch root; adapted from w17g/tusplice.sh). For each TU: build the CURRENT
# src/rom/<tu>.c and the SPLICED spl/src/rom/<tu>.c (tools/splice.py), both with IDO 7.1 (the segment's pinned
# compiler; master's TUs are byte-identical under 7.1, w17g tu71.log / w17k switch gate) through asm-processor like
# the Makefile ROM-TU rule; compare .text words + relocations (type, symbol) and .data/.rodata/.bss contents.
set -u
CC=tools/ido-static-recomp/build71/out/cc
CF="-G 0 -mips2 -O2 -non_shared -Iinclude -Iinclude/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -Isrc/rom"
mkdir -p tuout
dump() { { mips-linux-gnu-objdump -dr -j .text $1 | sed -n '/^Disassembly/,$p' | grep -v '^$\|>:$\|^Disassembly' \
  | sed 's/^ *[0-9a-f]*:\t//' | awk -F'\t' '/R_MIPS/{print $2; next}{print $1}' | sed 's/ *$//';
  for s in .data .rodata .bss; do mips-linux-gnu-objdump -s -j $s $1 2>/dev/null | sed -n '/^Contents/,$p'; done; }; }
for t in "$@"; do
  python3 tools/asm-processor/build.py $CC -- mips-linux-gnu-as -march=vr4300 -mabi=32 -Iinclude -- -c $CF -o tuout/$t.orig.o src/rom/$t.c > tuout/$t.orig.log 2>&1 || { echo "$t: ORIG BUILD FAIL"; tail -3 tuout/$t.orig.log; continue; }
  python3 tools/asm-processor/build.py $CC -- mips-linux-gnu-as -march=vr4300 -mabi=32 -Iinclude -- -c $CF -o tuout/$t.spl.o spl/src/rom/$t.c > tuout/$t.spl.log 2>&1 || { echo "$t: SPLICED BUILD FAIL"; grep -i error tuout/$t.spl.log | head -3; continue; }
  dump tuout/$t.orig.o > tuout/$t.orig.dump; dump tuout/$t.spl.o > tuout/$t.spl.dump
  n=$(grep -c . tuout/$t.orig.dump)
  if cmp -s tuout/$t.orig.dump tuout/$t.spl.dump; then echo "$t: IDENTICAL ($n lines: .text words+relocs, data sections)"; else
    echo "$t: DIFFERENT ($(diff tuout/$t.orig.dump tuout/$t.spl.dump | grep -c '^[<>]') changed lines of $n)"; diff tuout/$t.orig.dump tuout/$t.spl.dump | head -8; fi
done
