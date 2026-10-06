#!/bin/sh
# udis.sh FUNC : disassemble FUNC from the last unit build
mips-linux-gnu-objdump -d --no-show-raw-insn -M no-aliases,reg-names=numeric /home/cburnes/projects/rush2049-decomp/build/blob_unit/w10g/unit.o --disassemble="$1" 2>/dev/null | tail -n +7
