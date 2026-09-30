#!/bin/sh
# usage: s.sh file fn [flags]
cd /home/user/SFRush2049-decomp
F=${3:--O2}
python3 tools/cloud/score.py fn cloud/work/newtargets_hi/$1 $2 --flags "-g0 $F -mips2 -G 0 -non_shared"
