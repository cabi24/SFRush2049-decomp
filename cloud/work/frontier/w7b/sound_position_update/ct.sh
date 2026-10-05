#!/bin/sh
# ct.sh CAND LABEL [PROC] -- ctrace with both names scored
D=$(pwd); cd /home/cburnes/projects/rush2049-decomp
cand=$(cd $D; realpath $1); lab=$2; proc=$3
python3 -m tools.conveyor.pipeline.blob_unit --tag w7b score sound_position_update func_80095198 --with "$cand" 2>&1 | grep -E "FAIL|EQUAL" | head -1 | cut -c1-90
ssh watchman2 "cd ~/rush2049/scratch/frontier/w7b && rm -rf st_$lab && mkdir st_$lab && cp ../unit/w7b/stage/merged ../unit/w7b/stage/st st_$lab/"
if [ -z "$proc" ]; then
  ssh watchman2 "cd ~/rush2049/scratch/frontier/w7b && ./tr.sh st_$lab \$PWD/st_$lab/all.txt && echo traced"
else
  ssh watchman2 "cd ~/rush2049/scratch/frontier/w7b && ./tr.sh st_$lab \$PWD/st_$lab/d.txt CDX_PROC=$proc CDX_DETAIL_WEB=all && ./sum.sh st_$lab/d.txt"
fi
