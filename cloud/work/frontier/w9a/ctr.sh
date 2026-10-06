#!/bin/bash
# ctr.sh body.c LABEL [PROC] : unit score + snapshot + trace
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w9a
cat $W/pre.c $1 $W/post.c > $W/steering_sensitivity/_u.c
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w9a score steering_sensitivity --with $W/steering_sensitivity/_u.c 2>&1 | grep -E "EQUAL|FAIL" | head -1
lab=$2; proc=$3
ssh watchman2 "cd ~/rush2049/scratch/frontier/w9a/tr && rm -rf st_$lab && mkdir st_$lab && cp ../../unit/w9a/stage/merged ../../unit/w9a/stage/st st_$lab/"
if [ -z "$proc" ]; then
 ssh watchman2 "cd ~/rush2049/scratch/frontier/w9a/tr && ./tr9.sh st_$lab \$PWD/st_$lab/all.txt && grep -v procindex st_$lab/all.txt | grep -v p1cand > st_$lab/dec.txt; echo traced"
else
 ssh watchman2 "cd ~/rush2049/scratch/frontier/w9a/tr && ./tr9.sh st_$lab \$PWD/st_$lab/d.txt CDX_PROC=$proc CDX_DETAIL_WEB=all && sh ./sum.sh st_$lab/d.txt"
fi
