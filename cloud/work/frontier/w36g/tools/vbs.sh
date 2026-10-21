#!/bin/bash
# vbs.sh VARIANTDIR FN [flags] : score single-file variants on the builder (image A)
V=$1; FN=$2; FL=${3:-"-g0 -O3 -mips2 -G 0 -non_shared"}
VB=$(basename $V)
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w36g/v/$VB; mkdir -p ~/rush2049/scratch/frontier/w36g/v"
scp -q -r $V watchman2:rush2049/scratch/frontier/w36g/v/$VB
scp -q $(dirname $0)/vbs.py watchman2:rush2049/scratch/frontier/w36g/vbs.py
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36g && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 vbs.py v/$VB $FN '$FL'"
