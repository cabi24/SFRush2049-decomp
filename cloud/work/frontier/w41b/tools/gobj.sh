#!/bin/sh
# gobj.sh GROUPDIR(local) IMG LABEL : compile a (multi-file) group on the builder like score.py group, fetch the
# object to the scratchpad as LABEL.o and print an aligned diff of FUNC (env FN) via dd.py --obj
set -e
D=$1; IMG=$2; L=$3; S=rush2049/scratch/frontier/w41b
ssh watchman2 "rm -rf ~/$S/cand/$L && mkdir -p ~/$S/cand/$L" && scp -q $D/* watchman2:$S/cand/$L/
ssh watchman2 "cd ~/$S && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 objsyms.py cand/$L $IMG /tmp/w41b_$L.o >/dev/null"
O=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/be6c3ef7-ec10-405a-a401-0caaa1bc1bac/scratchpad/$L.o
scp -q watchman2:/tmp/w41b_$L.o $O
[ -n "$FN" ] && cd /home/cburnes/projects/rush2049-decomp && python3 cloud/work/frontier/w41b/tools/dd.py x $FN --img $IMG --obj $O --ctx ${CTX:-0} 2>&1 | head -${N:-60}
