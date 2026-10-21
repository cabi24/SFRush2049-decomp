#!/bin/bash
# usage: ob.sh VARDIR(abs or rel to repo) TARGET(ovl_a|ovl_b) FUNCS [--group lane-rel-json] [obatch args]
cd /home/cburnes/projects/rush2049-decomp || exit 1
L=cloud/work/frontier/w36x; S=rush2049/scratch/frontier/w36x
D=$1; V=$(realpath --relative-to=$L $1); T=$2; F=$3; shift 3
rsync -aq --exclude 'score.txt' $L/ watchman2:$S/lane/ || exit 1
ssh watchman2 "cd ~/$S && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 lane/tools/obatch.py lane/$V asm/us/$T $F $*" --top 100000 > $D/score.txt; head -${TOP:-15} $D/score.txt; echo ...; awk "{print \$2}" $D/score.txt | sort | uniq -c | sort -k2 -n | head -8
