#!/bin/sh
# Score a group dir on the builder: rg.sh LOCAL_GROUP_DIR ovl_a|ovl_b [--claims]
# Copies the dir to ~/rush2049/scratch/frontier/w38b/cand/<label>/ and runs score.py group.
set -e
D="$1"; IMG="$2"; shift 2
LABEL=$(echo "$D" | sed 's#/*$##; s#^.*cloud/work/frontier/w38b/##; s#/#__#g')
R=rush2049/scratch/frontier/w38b
ssh watchman2 "rm -rf ~/$R/cand/$LABEL && mkdir -p ~/$R/cand/$LABEL"
scp -q "$D"/* watchman2:$R/cand/$LABEL/
ssh watchman2 "cd ~/$R && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group cand/$LABEL --targets asm/us/$IMG $*; echo exit=\$?"
