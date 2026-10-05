#!/bin/sh
# usage: ctrace.sh NAME CAND.c LABEL [PROC] -- score NAME with CAND in the unit (tag w4a), snapshot the merged
# ucode on the builder as ~/rush2049/scratch/frontier/w4a/st_LABEL, run the instrumented uopt (globalcolor
# profile, built in ~/rush2049/scratch/frontier/w4a/uopt) and print one line per colouring decision.
# Without PROC prints the per-proc decision counts (find the ordinal by diffing two labels: pdiff.sh).
D=$(pwd); cd /home/cburnes/projects/rush2049-decomp
name=$1; cand=$(cd $D; realpath $2); lab=$3; proc=$4
python3 -m tools.conveyor.pipeline.blob_unit --tag w4a score $name --with "$cand" 2>&1 | grep -E "EQUAL|FAIL" | head -1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w4a && rm -rf st_$lab && mkdir st_$lab && cp ../unit/w4a/stage/merged ../unit/w4a/stage/st st_$lab/"
if [ -z "$proc" ]; then
  ssh watchman2 "cd ~/rush2049/scratch/frontier/w4a && ./tr.sh st_$lab \$PWD/st_$lab/all.txt && grep -v procindex st_$lab/all.txt | grep -E 'p[12]dec' | awk '{print \$4}' | sort | uniq -c | sort -k2 > st_$lab/counts.txt; echo traced"
else
  ssh watchman2 "cd ~/rush2049/scratch/frontier/w4a && ./tr.sh st_$lab \$PWD/st_$lab/d.txt CDX_PROC=$proc CDX_DETAIL_WEB=all && ./sum.sh st_$lab/d.txt"
fi
