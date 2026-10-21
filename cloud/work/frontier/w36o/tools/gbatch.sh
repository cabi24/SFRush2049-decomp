#!/bin/bash
# gbatch.sh GROUPDIR MEMBERFILE VARDIR FUNC: score each VARDIR/v*.c as MEMBERFILE inside GROUPDIR on watchman2 (serial)
g=${1%/}; mf=$2; vd=${3%/}; fn=$4
gn=$(basename $g); vn=$(basename $vd)
R=rush2049/scratch/frontier/w36o/gbatch/$vn
ssh watchman2 "rm -rf ~/$R; mkdir -p ~/$R/vars"
scp -q -r $g watchman2:$R/grp || exit 1
scp -q $vd/v*.c watchman2:$R/vars/ || exit 1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36o && export IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; for f in ~/$R/vars/v*.c; do cp \$f ~/$R/grp/$mf; r=\$(python3 tools/cloud/score.py group ~/$R/grp 2>&1 | awk '/^$fn:/{f=1} f&&/words differ|MATCH/{print; exit}'); echo \"\$(basename \$f) \$r\"; done" | sed 's/(4 section.*//' | sort -t' ' -k2 -n > $vd/score.txt
head -${TOP:-15} $vd/score.txt
