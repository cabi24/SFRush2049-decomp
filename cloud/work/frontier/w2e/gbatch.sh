#!/bin/sh
# usage: gbatch.sh DIR NAME -- DIR contains group dirs; score NAME in each on the builder
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w2e/cand/gb; mkdir -p ~/rush2049/scratch/frontier/w2e/cand/gb"
tar -C "$1" -cf - . | ssh watchman2 "tar -C ~/rush2049/scratch/frontier/w2e/cand/gb -xf -"
ssh watchman2 "cd ~/rush2049/scratch/frontier/w2e && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 cand/gbatch_remote.py cand/gb $2"
