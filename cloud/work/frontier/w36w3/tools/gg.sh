#!/bin/bash
# usage: gg.sh GROUPDIR [gscore args...]  -> group compile + per-function rows (+ --diff NAME)
D=$1; shift
S=watchman2:rush2049/scratch/frontier/w36w3
rsync -aq /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36w3/tools/gscore.py $S/tools/gscore.py
rsync -aq --delete $D/ $S/cand/gg_$(basename $D)/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36w3 && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido timeout 600 python3 tools/gscore.py group cand/gg_$(basename $D) $*"
