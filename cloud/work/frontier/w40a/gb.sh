#!/bin/bash
# usage: gb.sh HARNESS_DIR VARIANT_DIR FILE NAME[,NAME..] [--jobs N]  -> gbatch.py on the builder (scratch w40a)
H=$1; V=$2; F=$3; N=$4; shift 4
S=rush2049/scratch/frontier/w40a
HB=$(basename $H); VB=$(echo $V | tr / _)
ssh watchman2 "rm -rf ~/$S/cand/h/$HB ~/$S/cand/v/$VB; mkdir -p ~/$S/cand/h ~/$S/cand/v"
scp -q -r $H watchman2:$S/cand/h/$HB
scp -q -r $V watchman2:$S/cand/v/$VB
scp -q /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w40a/tools/gbatch.py watchman2:$S/gbatch.py
ssh watchman2 "cd ~/$S && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 gbatch.py cand/h/$HB cand/v/$VB $F $N $*" | tee $V/score.txt | head -${TOPN:-30}
