#!/bin/bash
# usage: rc.sh GROUPDIR FUNC DISFILE [roles args]  -> dump our FUNC rows and compare FP roles against retail DISFILE
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36h
S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad
D=$1; F=$2; R=$3; shift 3
O=$S/rc_$(basename $D)_$F.txt
$W/tools/g.sh $D --names $F --got $F:0:6000 2>&1 | grep -E '^ *[0-9]+ [a-z]' > $O
python3 $W/tools/roles.py $W/$R $O "$@"
