#!/bin/bash
# usage: ob2.sh VARDIR(rel to repo) ovl_a|ovl_b FUNC[,CHECK..] --group lane/REL/group.json [obatch2 args]
cd /home/cburnes/projects/rush2049-decomp || exit 1
L=cloud/work/frontier/w41b; S=rush2049/scratch/frontier/w41b
D=$1; V=$(realpath --relative-to=$L $1); T=$2; F=$3; shift 3
rsync -aq --exclude 'score*.txt' $L/ watchman2:$S/lane/ || exit 1
ssh watchman2 "cd ~/$S && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 lane/tools/obatch2.py lane/$V asm/us/$T $F $* --top 100000" > $D/score2.txt; head -${TOP:-25} $D/score2.txt
