#!/bin/sh
# usage: full.sh body.c [full.py args] -- csm prefix + body, aligned diff of camera_scene_manager (group keep)
D=$(dirname $0); b="$1"; shift
cat $D/csm_prefix.c "$b" > $D/_cur.c
scp -q $D/_cur.c watchman2:rush2049/scratch/frontier/w9h/cand/_f.c && ssh watchman2 "cd ~/rush2049/scratch/frontier/w9h && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 cand/full.py cand/_f.c camera_scene_manager --flags '-g0 -O3 -mips2 -G 0 -non_shared' --keep camera_scene_manager,func_800C3578 $*"
