#!/bin/sh
# usage: diag.sh NAME CAND.c [diagnose args] -- score NAME in the whole-program unit with CAND.c, build a retail
# target object from build/game_code.bin and run the vendored workbench diagnose on the pair.
cd /home/cburnes/projects/rush2049-decomp
name="$1"; cand="$2"; shift; shift
S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/diag_w8d
mkdir -p $S
python3 -m tools.conveyor.pipeline.blob_unit --tag w8d score $name --with "$cand" 2>&1 | grep -E "EQUAL|FAIL" | head -2
hdr=$(python3 cloud/work/tools/tdis.py $name | head -1)
addr=$(echo "$hdr" | sed 's/.*@ 0x\([0-9A-Fa-f]*\).*/\1/'); n=$(echo "$hdr" | sed 's/.*(\([0-9]*\) words).*/\1/')
python3 - "$addr" "$n" "$name" > $S/t.s <<'P'
import sys,struct
a=int(sys.argv[1],16); n=int(sys.argv[2]); d=open('build/game_code.bin','rb').read()[a-0x80086A50:a-0x80086A50+4*n]
print('.set noreorder\n.text\n.globl %s\n.type %s,@function\n%s:' % (sys.argv[3],sys.argv[3],sys.argv[3]))
for i in range(n): print('.word 0x%08x' % struct.unpack('>I',d[4*i:4*i+4])[0])
print('.size %s,.-%s' % (sys.argv[3],sys.argv[3]))
P
mips-linux-gnu-as -march=vr4300 -mabi=32 -EB -o $S/t.o $S/t.s
python3.11 tools/workbench.py diagnose $S/t.o build/blob_unit/w8d/unit.o --function $name --objdump mips-linux-gnu-objdump --pager never --color never "$@" 2>&1
