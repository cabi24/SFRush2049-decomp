#!/bin/bash
# dis.sh file.c [flags]  -> disassembly of compiled object
R=/home/user/SFRush2049-decomp
T=$(mktemp -d)
$R/tools/cloud/ido/cc -c ${2:--g0 -O2 -mips2 -G 0 -non_shared} -Wab,-r4300_mul -o $T/o.o "$1" 2>&1 | head -20
mips-linux-gnu-objdump -d -M no-aliases,reg-names=32 $T/o.o | sed -n '7,$p'
