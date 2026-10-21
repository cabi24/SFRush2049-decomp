#!/bin/bash
# usage: ob.sh VARDIR FN[,FN2] [--group GROUPDIR(on Pi) --file shots.c] [--jobs 2 --top 20]
# Scores every VARDIR/*.c on the builder (image B targets). With --group, each variant replaces FILE in the group.
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36h
S=watchman2:rush2049/scratch/frontier/w36h
D=$1; F=$2; shift 2
args=()
while [ $# -gt 0 ]; do
  if [ "$1" = "--group" ]; then rsync -aq --delete $2/ $S/cand/grp_$(basename $2)/; args+=(--group cand/grp_$(basename $2)); shift 2;
  else args+=("$1"); shift; fi
done
rsync -aq $W/tools/obscore.py $W/tools/gscore.py $S/tools/
b=$(basename $D)
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w36h/vb/$b && mkdir -p ~/rush2049/scratch/frontier/w36h/vb/$b"
rsync -aq --include='*.c' --exclude='*' $D/ $S/vb/$b/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36h && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/obscore.py vb/$b $F ${args[*]}" | tee $D/score.txt
