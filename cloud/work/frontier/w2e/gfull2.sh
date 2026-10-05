#!/bin/sh
# usage: gfull2.sh GROUPDIR NAME [--all] -- aligned diff of NAME for a (multi-file) group dir
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w2e/cand/gf; mkdir -p ~/rush2049/scratch/frontier/w2e/cand/gf"
tar -C "$1" -cf - . | ssh watchman2 "tar -C ~/rush2049/scratch/frontier/w2e/cand/gf -xf -"
name="$2"; shift; shift
ssh watchman2 "cd ~/rush2049/scratch/frontier/w2e && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 cand/gfull_remote.py cand/gf $name $*"
