#!/bin/bash
# mk.sh name [flags]  -> builds name.c from pre.h+body/name.c, scores
cd /home/user/SFRush2049-decomp/cloud/work/newtargets_lo
n=$1; shift
F="${1:--g0 -O2 -mips2 -G 0 -non_shared}"
cat pre.h body/$n.c > $n.c
(cd /home/user/SFRush2049-decomp && python3 tools/cloud/score.py fn cloud/work/newtargets_lo/$n.c $n --flags "$F" 2>&1) | tail -${LINES_N:-6}
