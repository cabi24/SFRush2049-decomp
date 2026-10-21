#!/bin/bash
# g.sh LOCALGROUPDIR FN [adiff args...] : push a group dir to the builder and print an aligned diff of FN
D=$1; shift; FN=$1; shift
B=$(basename $D)
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w36g/cand/$B" && scp -q -r $D watchman2:rush2049/scratch/frontier/w36g/cand/$B
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36g && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 adiff.py x $FN --group cand/$B $*"
