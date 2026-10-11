#!/bin/sh
# Whole-segment check of 0xe4f0 as a ROM TU at -mips3 -32 -O1 through tools/asm-processor (scratch only).
# Pi:      sh cloud/work/frontier/w16g/tu_e4f0.sh prep DIR      (writes DIR/src/rom/lib_e4f0.c, the osCauseGet
#                                                               passthrough cut from asm/us/E4F0.s, expected words)
# builder: sh tu_e4f0.sh build DIR IDO_DIR                      (asm-processor build.py exactly as the Makefile rule)
set -eu
case $1 in
prep) D=$2; mkdir -p $D/asm/us/nonmatchings/rom/lib_e4f0 $D/src/rom $D/include
  sed -n '/^nonmatching osCauseGet/,/^endlabel osCauseGet/p' asm/us/E4F0.s > $D/asm/us/nonmatchings/rom/lib_e4f0/osCauseGet.s
  cp -r include/* $D/include/; cp src/rom/rom_tu.h $D/src/rom/; cp -r tools/asm-processor $D/
  python3 - "$D" <<'PY'
import re, sys
from pathlib import Path
sys.path.insert(0, '.')
from tools.conveyor.seeds.extract_candidates import extract_named_function
D = Path(sys.argv[1]); S = Path('cloud/work/frontier/w16g/sources')
out = ['#include "rom_tu.h"', '']
for f in "__lshrdi3 __umoddi3 __udivdi3 __ashldi3 __umoddi3_alt __divdi3 __muldi3 __qdivrem __moddi3 __ashrdi3".split():
    out += [extract_named_function(S / (f + '.c'), f).rstrip(), '']
out.append('#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_e4f0/osCauseGet.s")')
(D / 'src/rom/lib_e4f0.c').write_text('\n'.join(out) + '\n')
w = [m.group(1) for l in Path('asm/us/E4F0.s').read_text().splitlines()
     for m in [re.match(r'\s*/\* [0-9A-F]+ [0-9A-F]{8} ([0-9A-F]{8}) \*/', l)] if m]
(D / 'expected.txt').write_text('\n'.join(w) + '\n')
PY
  ;;
build) D=$2; IDO=$3; cd $D
  python3 asm-processor/build.py $IDO/cc -- mips-linux-gnu-as -march=vr4300 -mabi=32 -Iinclude -- -c -G 0 -mips3 -32 -O1 \
    -non_shared -Iinclude -Iinclude/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -Isrc/rom -o lib_e4f0.o src/rom/lib_e4f0.c
  mips-linux-gnu-objcopy -O binary -j .text lib_e4f0.o text.bin
  python3 -c "
import struct
b=open('text.bin','rb').read(); w=['%08X'%x for x in struct.unpack('>%dI'%(len(b)//4),b)]; e=open('expected.txt').read().split()
print('segment 0xe4f0 words', len(e), 'object', len(w), 'IDENTICAL' if w[:len(e)]==e and set(w[len(e):])<={'00000000'} else 'DIFFERENT')"
  mips-linux-gnu-readelf -h lib_e4f0.o | grep Flags
  echo 'int f(int x){return x+1;}' > m2.c; $IDO/cc -c -G 0 -mips2 -O2 -non_shared -o m2.o m2.c
  mips-linux-gnu-ld -r -o both.o m2.o lib_e4f0.o && echo 'ld -r with a -mips2 object: ok' || echo 'ld -r with a -mips2 object: FAILS'
  ;;
esac
