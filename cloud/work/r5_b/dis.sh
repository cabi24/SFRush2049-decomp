#!/bin/bash
# dis.sh file.c flags...
f=$1; shift
D=/home/user/SFRush2049-decomp/tools/cloud/ido
$D/cc -c "$@" -Wab,-r4300_mul -o /tmp/claude-0/o_$$.o $f 2>&1 | head -5
mips-linux-gnu-objdump -d -M no-aliases,reg-names=numeric /tmp/claude-0/o_$$.o 2>/dev/null | head -${N:-120} || true
