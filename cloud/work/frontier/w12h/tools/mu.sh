#!/bin/bash
# mu.sh BODY NAME CONSTS [blob_unit args]: unit score (tag w12h), cfe Udef of body, merged Udef of proc containing CONSTS
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12h
S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/w12h; mkdir -p $S
b=$(realpath $1); n=$2; c=$3; shift 3; bn=$(basename $b .c)
r=$(cd /home/cburnes/projects/rush2049-decomp && python3 -m tools.conveyor.pipeline.blob_unit --tag w12h --jobs 2 score $n "$@" --with $b 2>&1 | grep -E "EQUAL|FAIL|rror|differ in" | head -3 | tr '\n' ' ')
scp -q watchman2:rush2049/scratch/frontier/unit/w12h/stage/c_$bn.u $S/c_$bn.u
scp -q watchman2:rush2049/scratch/frontier/unit/w12h/stage/merged $S/merged_$bn
echo "$bn: $r | $(python3 $W/tools/cfedef.py $S/c_$bn.u) | merged $(python3 $W/tools/udump.py $S/merged_$bn $c 2>/dev/null | grep -m1 'Udef Mmt' | awk '{print $NF, $(NF-1)}')"
