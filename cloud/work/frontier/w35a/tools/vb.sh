#!/bin/bash
# vb.sh <local variant dir> NAMES...  : scp variants to builder and run vb.py
D=$1; shift
B=$(basename $D)
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w35a/v/$B; mkdir -p ~/rush2049/scratch/frontier/w35a/v/$B"
scp -q $D/*.c watchman2:rush2049/scratch/frontier/w35a/v/$B/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w35a && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 vb.py parts v/$B $*"
