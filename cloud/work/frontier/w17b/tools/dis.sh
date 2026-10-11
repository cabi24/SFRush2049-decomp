#!/bin/sh
# dis.sh NAME [OBJ]: disassemble NAME from the unit object (default build/blob_unit/w17b/unit.o)
O=${2:-/home/cburnes/projects/rush2049-decomp/build/blob_unit/w17b/unit.o}
mips-linux-gnu-objdump -d --no-show-raw-insn -M no-aliases,gpr-names=numeric $O 2>/dev/null >/dev/null
mips-linux-gnu-objdump -d --no-show-raw-insn $O | awk -v n="<$1>:" '$2==n{p=1;next} p&&/^$/{exit} p{print}'
