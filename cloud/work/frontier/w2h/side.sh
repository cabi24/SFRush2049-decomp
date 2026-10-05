#!/bin/sh
# usage: side.sh NAME cand.c [lines] -- unit-score then raw side-by-side (retail | unit), no alignment
cd /home/cburnes/projects/rush2049-decomp
S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad
python3 -m tools.conveyor.pipeline.blob_unit --tag w2h score $1 --with $2 2>&1 | grep -o "FAIL.*\|EQUAL.*"
mips-linux-gnu-objdump -d -z --disassemble=$1 build/blob_unit/w2h/unit.o | awk 'NR>7{ $1=""; $2=""; print}' | sed 's/ <.*//' > $S/side_b.s
python3 cloud/work/tools/tdis.py $1 | awk 'NR>1{$1=""; print}' | sed 's/ <.*//' > $S/side_t.s
paste -d'|' $S/side_t.s $S/side_b.s | awk -F'|' '{m=($1==$2)?" ":"*"; printf "%s %-36s| %s\n", m, $1, $2}' | head -${3:-400}
