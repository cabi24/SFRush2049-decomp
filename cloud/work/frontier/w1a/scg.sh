#!/bin/sh
# usage: scg.sh GROUPDIR TAG -- strict-score an -O3 group dir on the builder (w1a copy)
ssh -o BatchMode=yes watchman2 "rm -rf ~/rush2049/scratch/frontier/w1a/groups/$2; mkdir -p ~/rush2049/scratch/frontier/w1a/groups/$2"
scp -q "$1"/* watchman2:rush2049/scratch/frontier/w1a/groups/$2/
ssh -o BatchMode=yes watchman2 "cd ~/rush2049/scratch/frontier/w1a && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group groups/$2"
