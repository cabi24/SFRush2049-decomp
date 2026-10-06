#!/bin/bash
# ctr.sh NAME body.c LABEL [PROC] : unit score (us.sh), snapshot merged ucode, run traced uopt
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w10c
name=$1; b=$2; lab=$3; proc=$4
$W/us.sh $b $name | grep -E "EQUAL|FAIL" | head -1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w10c/tr && rm -rf st_$lab && mkdir st_$lab && cp ../../unit/w10c/stage/merged ../../unit/w10c/stage/st st_$lab/"
if [ -z "$proc" ]; then
 ssh watchman2 "cd ~/rush2049/scratch/frontier/w10c/tr && ./tr.sh st_$lab \$PWD/st_$lab/all.txt && grep -v procindex st_$lab/all.txt | grep -E 'p[12]dec' | awk '{print \$4}' | sort | uniq -c | sort -k2 > st_$lab/counts.txt; echo traced"
else
 ssh watchman2 "cd ~/rush2049/scratch/frontier/w10c/tr && ./tr.sh st_$lab \$PWD/st_$lab/d.txt CDX_PROC=$proc CDX_DETAIL_WEB=all && sh ./sum.sh st_$lab/d.txt"
fi
