#!/bin/sh
# gt.sh SRC.c : score group with buffer_swap.c replaced
cd /home/cburnes/projects/rush2049-decomp
G=cloud/work/frontier/w23d/groups/codex_visual_tire_module_20261002
rsync -a --delete $G/ watchman2:rush2049/scratch/frontier/w23d/grp_t/
scp -q $1 watchman2:rush2049/scratch/frontier/w23d/grp_t/buffer_swap.c
ssh watchman2 'cd ~/rush2049/scratch/frontier/w23d && IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group grp_t 2>&1' | grep -v '^    +0x' | grep -A1 '^buffer_swap:$' | tail -1
