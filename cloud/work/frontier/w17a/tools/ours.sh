#!/bin/sh
# ours.sh [OBJ] : disassemble camera_play_script from the unit object (default build/blob_unit/w17a/unit.o), with offsets
O=${1:-build/blob_unit/w17a/unit.o}
mips-linux-gnu-objdump -d --no-show-raw-insn -M gpr-names=32 "$O" 2>/dev/null | python3 -c '
import sys,re
f=0;b=None
for l in sys.stdin:
    if l.rstrip().endswith("<camera_play_script>:"): f=1; continue
    if f:
        m=re.match(r"\s*([0-9a-f]+):\s+(.*)",l)
        if not m:
            if l.strip()=="" : break
            continue
        a=int(m.group(1),16); b=a if b is None else b
        print("+%03x %s"%(a-b,m.group(2).strip()))
'
