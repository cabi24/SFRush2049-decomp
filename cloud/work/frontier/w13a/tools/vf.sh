#!/bin/sh
# vf.sh NAME : v.sh var/NAME.c NAME, then force the table(type1)/const4(type2) a1/a2 tie to retail (table a2, 4 a1)
cd /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w13a
tools/v.sh var/$1.c $1 | grep -v "locked\|score:"
cd /home/cburnes/projects/rush2049-decomp; export TAG=w13a SCR=rush2049/scratch/frontier/w13a; T=cloud/work/frontier/tools/trace
$T/ctrace.sh func_800E05F0 $1 > build/trace/w13a/ct_$1.txt 2>&1
t=$(grep -E "save=3.333333 +nocs=3 .*-> a[12] +type=1 " build/trace/w13a/ct_$1.txt | awk '{print $2}' | tr -d w)
c=$(grep -E "save=3.333333 +nocs=3 .*-> a[12] +type=2 " build/trace/w13a/ct_$1.txt | awk '{print $2}' | tr -d w)
echo "   force table=w$t const=w$c: $($T/force.sh $1 func_800E05F0 p1:w$t=c5,p1:w$c=c4 2>&1 | tail -1)"
