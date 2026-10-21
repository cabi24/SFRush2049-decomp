#!/bin/bash
cd /home/cburnes/projects/rush2049-decomp || exit 1
L=cloud/work/frontier/w36e
rsync -aq --exclude 'v/' $L/ watchman2:rush2049/scratch/frontier/w36e/lane/ || exit 1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36e && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group lane/$1 --targets asm/us/ovl_a --claims"
