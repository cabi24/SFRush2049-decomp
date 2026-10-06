#!/bin/bash
# usage: udiff.sh NAME  -- side-by-side diff of unit.o body vs retail (tdis)
N=$1; R=/home/cburnes/projects/rush2049-decomp; S=/tmp/claude-1000/udiff; mkdir -p $S
mips-linux-gnu-objdump -d -z --no-show-raw-insn -M no-aliases,reg-names=numeric $R/build/blob_unit/w9c/unit.o >/dev/null 2>&1
mips-linux-gnu-objdump -d -z --no-show-raw-insn $R/build/blob_unit/w9c/unit.o | awk -v n="<$N>:" '$2==n{f=1;next} f&&/^$/{exit} f' | sed -E 's/^ *[0-9a-f]+:\s*//; s/\s+/ /g; s/<[^>]*>//; s/ *$//' > $S/u.txt
python3 $R/cloud/work/tools/tdis.py $N | tail -n +2 | sed -E 's/^ *[0-9a-f]+: //; s/\s+/ /g; s/<[^>]*>//; s/ *$//' > $S/r.txt
# normalise: drop branch targets/addresses & lui/addr immediates
norm() { sed -E 's/0x[0-9a-f]+$/X/; s/(lui [a-z0-9]+,)0x[0-9a-f]+/\1H/' $1; }
norm $S/r.txt > $S/rn.txt; norm $S/u.txt > $S/un.txt
wc -l < $S/rn.txt; wc -l < $S/un.txt
diff $S/rn.txt $S/un.txt | head -${2:-60}
