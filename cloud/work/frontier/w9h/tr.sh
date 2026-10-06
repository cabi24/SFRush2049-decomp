#!/bin/sh
# usage: tr.sh FILE.c LABEL [PROC]  (FILE is inline-funcs+body; prefix prepended) -> colouring summary
D=$(dirname $0); f=$1; lab=$2; proc=$3
cat $D/csm_prefix.c $f > $D/_t.c
ssh watchman2 "mkdir -p ~/rush2049/scratch/frontier/w9h/cand/tr_$lab" && scp -q $D/_t.c watchman2:rush2049/scratch/frontier/w9h/cand/tr_$lab/group.c
if [ -z "$proc" ]; then
ssh watchman2 "cd ~/rush2049/scratch/frontier/w9h/cand && ./gtr.sh tr_$lab && grep -o 'proc=[0-9]*[^ ]* *[^ ]*' tr_$lab/trace.txt | sort | uniq -c | sort -rn | head -20; grep -m3 procindex tr_$lab/trace.txt"
else
ssh watchman2 "cd ~/rush2049/scratch/frontier/w9h/cand && ./gtr.sh tr_$lab CDX_PROC=$proc CDX_DETAIL_WEB=all && ./sum.sh tr_$lab/trace.txt"
fi
