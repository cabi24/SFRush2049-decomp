#!/bin/sh
# inst.sh file fn [flags]  -> copy to cloud/matches
F=${3:--O2}
R=/home/user/SFRush2049-decomp
(echo "/* flags: -g0 $F -mips2 -G 0 -non_shared */"; cat $R/cloud/work/newtargets_hi/$1) > $R/cloud/matches/$2.c
