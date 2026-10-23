#!/bin/bash
# usage: gd.sh GROUPDIR NAME [udiff args]  -> compile group on builder, aligned diff of NAME on the Pi
D=$1; N=$2; shift 2
B=$(basename $D)
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w40a/cand/g/$B; mkdir -p ~/rush2049/scratch/frontier/w40a/cand/g ~/rush2049/scratch/frontier/w40a/obj"
scp -q -r $D watchman2:rush2049/scratch/frontier/w40a/cand/g/$B
ssh watchman2 "cd ~/rush2049/scratch/frontier/w40a && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 gobj.py cand/g/$B obj/$B.o" || exit 1
mkdir -p /tmp/claude-1000/w40a_obj
scp -q watchman2:rush2049/scratch/frontier/w40a/obj/$B.o /tmp/claude-1000/w40a_obj/$B.o
python3 /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace/udiff.py $N --obj /tmp/claude-1000/w40a_obj/$B.o "$@"
