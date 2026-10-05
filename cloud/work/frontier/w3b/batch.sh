#!/bin/sh
# usage: batch.sh DIR NAME [bscore args] -- score every DIR/*.c on the builder as NAME (aligned + strict counts)
dir="$1"; name="$2"; shift; shift
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w3b/cand/b_$name; mkdir -p ~/rush2049/scratch/frontier/w3b/cand/b_$name"
tar -C "$dir" -cf - . | ssh watchman2 "tar -C ~/rush2049/scratch/frontier/w3b/cand/b_$name -xf -"
ssh watchman2 "cd ~/rush2049/scratch/frontier/w3b && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 cand/bscore.py cand/b_$name $name $*"
