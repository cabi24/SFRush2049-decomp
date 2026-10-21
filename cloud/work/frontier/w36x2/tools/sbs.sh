#!/bin/bash
# usage: sbs.sh group groups/DIR NAME [--norm] | sbs.sh fn FILE.c NAME [--norm]   (paths relative to lane dir)
# env TGT=asm/us/ovl_a (default)
cd /home/cburnes/projects/rush2049-decomp || exit 1
L=cloud/work/frontier/w36x2
B=watchman2:rush2049/scratch/frontier/w36x2
SP=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad/w36x2
TGT=${TGT:-asm/us/ovl_a}
rsync -aq --exclude 'dis/' --exclude '*.pyc' --exclude 'v/' $L/ $B/lane/ || exit 1
IDO='IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido'
J=lw_$$.json
P=lane/$2; case "$2" in v/*) P=$2;; esac
if [ "$1" = group ]; then
  ssh watchman2 "cd ~/rush2049/scratch/frontier/w36x2 && $IDO TGT=$TGT python3 lane/tools/gotwords.py group $P $3 $J" || exit 1
else
  FL="-g0 -O3 -mips2 -G 0 -non_shared"
  ssh watchman2 "cd ~/rush2049/scratch/frontier/w36x2 && $IDO TGT=$TGT python3 lane/tools/gotwords.py fn $P $3 '$FL' $J" || exit 1
fi
scp -q $B/$J $SP/$J && ssh watchman2 "rm -f ~/rush2049/scratch/frontier/w36x2/$J"
python3 $L/tools/sbsdiff.py $SP/$J "$3" "$4"
rm -f $SP/$J
