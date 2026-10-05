#!/bin/sh
# usage: udiff.sh NAME -- side-by-side of retail vs build/blob_unit/w2a/unit.o for NAME (run after blob_unit score)
cd /home/cburnes/projects/rush2049-decomp
python3 cloud/work/tools/tdis.py $1 | sed 1d | sed 's/^ *[0-9a-f]*: //; s/   <.*//' | tr '\t' ' ' > /tmp/claude-1000/_w2a_t.txt
mips-linux-gnu-objdump -d -r -M gpr-names=32 --disassemble=$1 build/blob_unit/w2a/unit.o | grep -P '^\s+[0-9a-f]+:\t' | cut -f3- | tr '\t' ' ' | sed 's/ <.*//' > /tmp/claude-1000/_w2a_u.txt
diff -y -W 100 /tmp/claude-1000/_w2a_t.txt /tmp/claude-1000/_w2a_u.txt
