#!/bin/bash
# usage: gsc.sh IMG GROUPDIR [FN]  -- score a group dir on the builder (w36f scratch); prints only FN's section (default whole output)
IMG=$1; D=$2; FN=$3
N=$(basename $D)
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w36f/cand/g_$N && mkdir -p ~/rush2049/scratch/frontier/w36f/cand/g_$N"
rsync -a $D/ watchman2:rush2049/scratch/frontier/w36f/cand/g_$N/
OUT=$(ssh watchman2 "cd ~/rush2049/scratch/frontier/w36f && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group cand/g_$N --targets asm/us/ovl_$IMG" 2>&1)
if [ -n "$FN" ]; then
  echo "$OUT" | awk -v fn="$FN:" '$0==fn{p=1;print;next} /^func_[0-9A-Fa-f]+:$/{p=0} /^Context/{p=0} p{print}'
else echo "$OUT"; fi
