#!/bin/bash
# vb.sh TEMPLATEDIR VARIANTDIR FN... : push template + variants to the builder and rank them
T=$1; V=$2; shift 2
TB=$(basename $T); VB=$(basename $V)
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w36g/t/$TB ~/rush2049/scratch/frontier/w36g/v/$VB; mkdir -p ~/rush2049/scratch/frontier/w36g/t ~/rush2049/scratch/frontier/w36g/v"
scp -q -r $T watchman2:rush2049/scratch/frontier/w36g/t/$TB
scp -q -r $V watchman2:rush2049/scratch/frontier/w36g/v/$VB
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36g && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 vb.py t/$TB v/$VB $*"
