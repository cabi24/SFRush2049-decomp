#!/bin/sh
# usage: [UARGS="--internal X"] ctrace.sh NAME CAND.c LABEL [PROC] -- score NAME with CAND in the unit (tag w11d),
# snapshot merged ucode as ~/rush2049/scratch/frontier/w11d/st_LABEL, run instrumented uopt; without PROC print
# per-proc decision counts file, with PROC one line per colouring decision.
D=$(pwd); cd /home/cburnes/projects/rush2049-decomp
name=$1; cand=$(cd $D; realpath $2); lab=$3; proc=$4
python3 -m tools.conveyor.pipeline.blob_unit --tag w11d score $name $UARGS --with "$cand" 2>&1 | grep -E "(EQUAL|FAIL) $name" | head -1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w11d && rm -rf st_$lab && mkdir st_$lab && cp ../unit/w11d/stage/merged ../unit/w11d/stage/st st_$lab/"
if [ -z "$proc" ]; then
  ssh watchman2 "cd ~/rush2049/scratch/frontier/w11d && ./tr.sh st_$lab \$PWD/st_$lab/all.txt && grep -v procindex st_$lab/all.txt | grep -E 'p[12]dec' | awk '{print \$4}' | sort | uniq -c | sort -k2 > st_$lab/counts.txt; grep -m3 procindex st_$lab/all.txt; echo traced"
else
  ssh watchman2 "cd ~/rush2049/scratch/frontier/w11d && ./tr.sh st_$lab \$PWD/st_$lab/d.txt CDX_PROC=$proc CDX_DETAIL_WEB=all && ./sum.sh st_$lab/d.txt"
fi
