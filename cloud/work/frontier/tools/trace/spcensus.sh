#!/bin/sh
# usage: TAG=.. spcensus.sh NAME [OBJ]  -- sp-offset census, retail (R) vs ours (U): frame, then "off,reg,reg..."
# for every sp-relative load/store. Shows which homes/temp slots moved. OBJ defaults to the last unit build.
. "$(dirname "$0")/lib/env.sh"
N=$1; O=${2:-$REPO/build/blob_unit/$TAG/unit.o}
mips-linux-gnu-objdump -d -z --no-show-raw-insn "$O" | awk -v n="<$N>:" '$2==n{f=1;next} f&&/^$/{exit} f' | sed -E 's/^ *[0-9a-f]+:\s*//; s/\s+/ /g' > $LOUT/sp_u.txt
python3 $REPO/cloud/work/tools/tdis.py $N | tail -n +2 | sed -E 's/^ *[0-9a-f]+: //; s/\s+/ /g' > $LOUT/sp_r.txt
c(){ grep -oE "[a-z0-9]+ [a-z0-9$]+,-?[0-9]+\(sp\)" $1 | sed -E 's/^([a-z0-9]+) ([a-z0-9$]+),(-?[0-9]+)\(sp\)/\3:\2/' | sort -n | uniq | awk -F: '{a[$1]=a[$1]","$2} END{for(k in a) print k a[k]}' | sort -n | tr '\n' ' '; echo; }
echo "R: $(grep -m1 'addiu sp' $LOUT/sp_r.txt)"; c $LOUT/sp_r.txt; echo "U: $(grep -m1 'addiu sp' $LOUT/sp_u.txt)"; c $LOUT/sp_u.txt
