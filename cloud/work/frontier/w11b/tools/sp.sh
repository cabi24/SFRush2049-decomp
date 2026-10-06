#!/bin/bash
# sp.sh NAME : sp-offset census (retail vs last w11b unit object), "off:regs"
R=/home/cburnes/projects/rush2049-decomp; N=$1; S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/w11b; mkdir -p $S
mips-linux-gnu-objdump -d -z --no-show-raw-insn $R/build/blob_unit/w11b/unit.o | awk -v n="<$N>:" '$2==n{f=1;next} f&&/^$/{exit} f' | sed -E 's/^ *[0-9a-f]+:\s*//; s/\s+/ /g' > $S/u.txt
python3 $R/cloud/work/tools/tdis.py $N | tail -n +2 | sed -E 's/^ *[0-9a-f]+: //; s/\s+/ /g' > $S/r_$N.txt
c(){ grep -oE "[a-z0-9]+ [a-z0-9$]+,-?[0-9]+\(sp\)" $1 | sed -E 's/^([a-z0-9]+) ([a-z0-9$]+),(-?[0-9]+)\(sp\)/\3:\2/' | sort -n | uniq | awk -F: '{a[$1]=a[$1]","$2} END{for(k in a) print k a[k]}' | sort -n | tr '\n' ' '; echo; }
echo "R: $(grep -m1 'addiu sp' $S/r_$N.txt)"; c $S/r_$N.txt; echo "U: $(grep -m1 'addiu sp' $S/u.txt)"; c $S/u.txt
