#!/bin/sh
# w20c whole-TU proof (builder, scratch repo root; w19a/tools/tusplice.sh + the Makefile's owned_data object
# normalisation). orig = the TU as committed on branch w20c (passthrough slots, owned companions; ORIG=dir),
# spl = promote_batch --splice-out TU (SPL=dir). Both IDO 7.1 through asm-processor like the ROM-TU rule; compare
# .text words + relocations (type, symbol) and .data/.rodata/.bss contents.
set -u
ORIG=${ORIG:-orig}; SPL=${SPL:-spl}
CC=tools/ido-static-recomp/build71/out/cc
CF="-G 0 -mips2 -O2 -non_shared -Iinclude -Iinclude/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -Isrc/rom"
mkdir -p tuout
dump() { { mips-linux-gnu-objdump -dr -j .text $1 | sed -n '/^Disassembly/,$p' | grep -v '^$\|>:$\|^Disassembly' \
  | sed 's/^ *[0-9a-f]*:\t//' | awk -F'\t' '/R_MIPS/{print $2; next}{print $1}' | sed 's/ *$//';
  for s in .data .rodata .bss; do mips-linux-gnu-objdump -s -j $s $1 2>/dev/null | sed -n '/^Contents/,$p'; done; }; }
for t in "$@"; do
  for k in orig spl; do
    d=$ORIG; [ $k = spl ] && d=$SPL
    cp $d/$t.c src/rom/$t.c
    python3 tools/asm-processor/build.py $CC -- mips-linux-gnu-as -march=vr4300 -mabi=32 -Iinclude -- -c $CF -o build/us/src/rom/$t.o src/rom/$t.c > tuout/$t.$k.log 2>&1 \
      && python3 -m tools.conveyor.pipeline.owned_data object src/rom/$t.c build/us/src/rom/$t.o >> tuout/$t.$k.log 2>&1 \
      && cp build/us/src/rom/$t.o tuout/$t.$k.o || { echo "$t: $k BUILD FAIL"; tail -3 tuout/$t.$k.log; }
  done
  cp $ORIG/$t.c src/rom/$t.c
  dump tuout/$t.orig.o > tuout/$t.orig.dump; dump tuout/$t.spl.o > tuout/$t.spl.dump
  n=$(grep -c . tuout/$t.orig.dump)
  if cmp -s tuout/$t.orig.dump tuout/$t.spl.dump; then echo "$t: IDENTICAL ($n lines: .text words+relocs, data sections)"; else
    echo "$t: DIFFERS ($(diff tuout/$t.orig.dump tuout/$t.spl.dump | grep -c '^[<>]') changed lines of $n):"; diff tuout/$t.orig.dump tuout/$t.spl.dump | grep '^[<>]'; fi
done
