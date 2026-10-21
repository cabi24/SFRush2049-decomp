#!/bin/bash
# usage: gb.sh VARDIR(relative to lane dir) GROUPJSON(relative to lane dir) FUNC...
cd /home/cburnes/projects/rush2049-decomp || exit 1
L=cloud/work/frontier/w35d
rsync -aq $L/ watchman2:rush2049/scratch/frontier/w35d/lane/ || exit 1
V=$1; J=$2; shift 2
ssh watchman2 "cd ~/rush2049/scratch/frontier/w35d && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 lane/gbatch.py lane/$V lane/$J $*"
