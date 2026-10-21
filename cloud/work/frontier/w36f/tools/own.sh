#!/bin/bash
# own.sh GROUPDIR NAME... : compile group on builder, fetch object, verify own data against private image A (Pi)
D=$1; shift
N=$(basename $D)
H=$(cd $(dirname $0) && pwd)
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w36f/cand/g_$N && mkdir -p ~/rush2049/scratch/frontier/w36f/cand/g_$N"
rsync -a $D/ watchman2:rush2049/scratch/frontier/w36f/cand/g_$N/
scp -q $H/gobj.py watchman2:rush2049/scratch/frontier/w36f/gobj.py
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36f && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 gobj.py cand/g_$N cand/g_$N.o"
mkdir -p $H/../obj && scp -q watchman2:rush2049/scratch/frontier/w36f/cand/g_$N.o $H/../obj/$N.o
python3 /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w35a/tools/owncheck.py $H/../obj/$N.o "$@"
