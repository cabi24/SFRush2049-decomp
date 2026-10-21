#!/bin/bash
# usage: gb.sh BASEGROUPDIR VARIANTDIR [bargs]  (base group dir on the Pi; variants *.c = replacement of func_803AD57C)
B=$(realpath $1); V=$(realpath $2); shift 2
N=$(basename $V); BN=$(basename $B)
SC=rush2049/scratch/frontier/w36f
ssh watchman2 "rm -rf ~/$SC/vb/$N ~/$SC/vb/base_$BN && mkdir -p ~/$SC/vb/$N ~/$SC/vb/base_$BN"
scp -q $(dirname $0)/gbatch.py watchman2:$SC/vb/gbatch.py
rsync -aq $B/ watchman2:$SC/vb/base_$BN/
rsync -aq --exclude='*.txt' --exclude='*.tsv' $V/ watchman2:$SC/vb/$N/
ssh watchman2 "cd ~/$SC && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 vb/gbatch.py vb/base_$BN vb/$N func_803AD57C a $*" | tee $V/score.txt
