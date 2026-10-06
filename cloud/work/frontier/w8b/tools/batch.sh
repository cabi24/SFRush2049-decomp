#!/bin/sh
# usage: batch.sh NAME FLAGS FILE.c... -- score each candidate standalone on the builder (w8b scratch), print summary line
name=$1; flags=$2; shift; shift
d=b_$$
ssh watchman2 "mkdir -p ~/rush2049/scratch/frontier/w8b/cand/$d"
tar -cf - "$@" | ssh watchman2 "tar -C ~/rush2049/scratch/frontier/w8b/cand/$d -xf -"
ssh watchman2 "cd ~/rush2049/scratch/frontier/w8b && for f in $*; do r=\$(IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py fn cand/$d/\$f $name --flags '$flags' 2>&1 | grep -E 'MATCH|differ|rror' | head -1); echo \"\$f: \$r\"; done; rm -rf cand/$d"
