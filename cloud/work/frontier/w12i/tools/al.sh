#!/bin/sh
# al.sh TAIL.c : score + every .alias/.noalias $8 line of E05F0's ugen listing with the preceding instruction
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12i
$W/tools/tv.sh $1 | head -1
n=$(basename $1 .c)
$W/tools/lst.sh $n func_800E05F0 > $W/var/lst_$n.s 2>/dev/null
echo "   A8: $(awk '/alias\t\$8,\$sp/{print "[" prev " | " $0 "]"} !/^\t\.(loc|livereg)/{prev=$0}' $W/var/lst_$n.s | tr '\t' ' ' | tr '\n' ' ')"
