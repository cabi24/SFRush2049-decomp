#!/bin/sh
# usage: grp.sh GROUPDIR -- score an IPA group on the builder
g=$(basename $(dirname "$1/x"))_$$
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w2a/cand/g_$g; mkdir -p ~/rush2049/scratch/frontier/w2a/cand/g_$g"
tar -C "$1" -cf - . | ssh watchman2 "tar -C ~/rush2049/scratch/frontier/w2a/cand/g_$g -xf -"
ssh watchman2 "cd ~/rush2049/scratch/frontier/w2a && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group cand/g_$g; rm -rf cand/g_$g"
