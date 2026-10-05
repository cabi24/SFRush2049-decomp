#!/bin/sh
# usage: gbatch.sh DIR NAME KEEP  -- score every DIR/*.c as -O3 group (keep list) on NAME
dir="$1"; name="$2"; keep="$3"
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w1d/cand/b_$name; mkdir -p ~/rush2049/scratch/frontier/w1d/cand/b_$name"
tar -C "$dir" -cf - . | ssh watchman2 "tar -C ~/rush2049/scratch/frontier/w1d/cand/b_$name -xf -"
ssh watchman2 "cd ~/rush2049/scratch/frontier/w1d && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 cand/bscore.py cand/b_$name $name --flags '-g0 -O3 -mips2 -G 0 -non_shared' --keep $keep --top ${4:-40}"
