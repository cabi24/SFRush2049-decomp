#!/bin/bash
# mu.sh BODY NAME CONSTS : unit score, then print cfe Udef of the body file and merged Udef of the proc containing CONSTS
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11b
S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/w11b; mkdir -p $S
b=$1; n=$2; c=${3:-2047,8192,4096}; bn=$(basename $b .c)
r=$($W/tools/us.sh $b $n | grep -E "EQUAL|FAIL|rror" | head -1)
scp -q watchman2:rush2049/scratch/frontier/unit/w11b/stage/c_$bn.u $S/c_$bn.u
scp -q watchman2:rush2049/scratch/frontier/unit/w11b/stage/merged $S/merged_$bn
echo "$bn: $r | $(python3 $W/tools/cfedef.py $S/c_$bn.u) | merged $(python3 $W/tools/udump.py $S/merged_$bn $c | grep -m1 'Udef Mmt' | awk '{print $NF, $(NF-1)}')"
