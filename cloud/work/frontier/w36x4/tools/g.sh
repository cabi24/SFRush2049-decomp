#!/bin/bash
# usage: g.sh GROUPDIR(on Pi) [gscore args...]   -> syncs dir to builder scratch and runs gscore group
D=$1; shift
S=watchman2:rush2049/scratch/frontier/w36x4
rsync -aq /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36x4/tools/gscore.py $S/tools/gscore.py
rsync -aq --delete $D/ $S/cand/$(basename $D)/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36x4 && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/gscore.py group cand/$(basename $D) $*"
