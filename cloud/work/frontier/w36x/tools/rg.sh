#!/bin/bash
# usage: rg.sh DIR [--claims]  -- copy group dir to builder and score (full output to DIR/score.txt)
d=$1; shift; n=$(basename $d); IMG=${IMG:-ovl_a}
S=rush2049/scratch/frontier/w36x
ssh watchman2 "mkdir -p $S/cand/$n" && scp -q $d/*.c $d/group.json watchman2:$S/cand/$n/ && \
ssh watchman2 "cd $S && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group cand/$n --targets asm/us/$IMG $*" > $d/score.txt 2>&1
echo "exit=$?" >> $d/score.txt
cut -c1-220 $d/score.txt | grep -v "^    +0x\|not verified"
