#!/bin/sh
# usage: tr.sh BODY.c LABEL [PROC] -- cps group (gprefix+sound+gtypes+body) traced colouring
D=$(cd $(dirname $0) && pwd); f=$1; lab=$2; proc=$3
sed 's/col->edge/col->pad42/' $f > $D/_gb.c
cat $D/gprefix.c $D/sound.c $D/gtypes.c $D/_gb.c > $D/_g.c
python3 -c "
import json;print('\n'.join(json.load(open('$D/g_c/group.json'))['keep']))" > $D/_keep.txt
ssh watchman2 "mkdir -p ~/rush2049/scratch/frontier/w9h/cand/tr_$lab" && scp -q $D/_g.c watchman2:rush2049/scratch/frontier/w9h/cand/tr_$lab/group.c && scp -q $D/_keep.txt watchman2:rush2049/scratch/frontier/w9h/cand/tr_$lab/keep.txt
if [ -z "$proc" ]; then
ssh watchman2 "cd ~/rush2049/scratch/frontier/w9h/cand && ./gtr2.sh tr_$lab && grep procindex tr_$lab/trace.txt"
else
ssh watchman2 "cd ~/rush2049/scratch/frontier/w9h/cand && ./gtr2.sh tr_$lab CDX_PROC=$proc CDX_DETAIL_WEB=all && ./sum.sh tr_$lab/trace.txt"
fi
