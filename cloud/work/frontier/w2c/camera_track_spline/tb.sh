#!/bin/sh
# usage: tb.sh DIR [bscore args] -- wrap each DIR/*.c body with head.h + stand-in caller and batch-score camera_track_spline
cd /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w2c/camera_track_spline
d="$1"; shift
rm -rf _tb; mkdir _tb
for f in $d/*.c; do (echo "/* flags */"; cat head.h; cat "$f"; cat standin.c) > _tb/$(basename $f); done
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w2c/cand/b_cts; mkdir -p ~/rush2049/scratch/frontier/w2c/cand/b_cts"
tar -C _tb -cf - . | ssh watchman2 "tar -C ~/rush2049/scratch/frontier/w2c/cand/b_cts -xf -"
ssh watchman2 "cd ~/rush2049/scratch/frontier/w2c && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 cand/bscore.py cand/b_cts camera_track_spline --flags '-g0 -O3 -mips2 -G 0 -non_shared' --keep camera_update $*"
