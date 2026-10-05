#!/bin/sh
# usage: pmap.sh file.c -- PrevMaxPath register mapping in the unit (loaded, rom, bssStart/bssEnd)
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w3c score PrevMaxPath --with "$1" --internal PrevMaxPath >/dev/null 2>&1
mips-linux-gnu-objdump -d -M gpr-names=32 --disassemble=PrevMaxPath build/blob_unit/w3c/unit.o | grep -P '^\s+[0-9a-f]+:' | cut -f3- | tr '\t' ' ' | grep -E "^(lb|subu|sb|move a0|addiu sp)" | tr '\n' ';'; echo
