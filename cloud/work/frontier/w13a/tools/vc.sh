#!/bin/sh
# vc.sh NAME : v.sh + model (web 2) t1/s3 costs and the a1/a2 tie webs
cd /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w13a
tools/v.sh var/$1.c $1 | grep -v "locked\|score:\|A8\|(ops)"
cd /home/cburnes/projects/rush2049-decomp; export TAG=w13a SCR=rush2049/scratch/frontier/w13a; T=cloud/work/frontier/tools/trace
$T/ctrace.sh func_800E05F0 $1 > build/trace/w13a/ct_$1.txt 2>&1
echo "   model: $(grep 'p1cost.*web=2 color=\(8\|17\) ' build/trace/w13a/$1.func_800E05F0.cdx | awk '{print $7,$9}' | tr '\n' ' ') | $(grep -E ' -> a[12] +type=[12] ' build/trace/w13a/ct_$1.txt | awk '{print $2,$10,$12}' | tr '\n' ' ')"
