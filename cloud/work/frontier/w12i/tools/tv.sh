#!/bin/sh
# tv.sh TAIL.c [funcs...]: mode.c = var/head.c + TAIL.c ; score whole component, print E05F0 summary + stack census
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12i
t=$1; shift
n=$(basename $t .c)
cat $W/var/head.c $t > $W/var/full_$n.c
cd $W && echo "V={'$n':[]}" > /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/e_$n.py && SLOT=mode.c SPC=1 XINT=${XINT:-func_800E0048} TAG=w12i python3 tools/rv.py var/full_$n.c func_800E05F0 /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/e_$n.py "$@" | sed 's/| FAIL func_800E05F0: \([0-9]* of [0-9]*\).*/| \1/; s/(words)\]//; s/(ops)\]//'
