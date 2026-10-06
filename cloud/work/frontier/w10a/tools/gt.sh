#!/bin/sh
# usage: gt.sh SRC.c LABEL PROC [KEEP] -- trace PROC in an -O3 group file; report in cloud/work/frontier/w10a/runs/LABEL
R=/home/cburnes/projects/rush2049-decomp
src=$1; lab=$2; proc=$3; K=${4:-zz_caller,zz_caller2,camera_update_c,select_screen_update}
OUT=$R/cloud/work/frontier/w10a/runs/$lab; mkdir -p $OUT
scp -q $R/cloud/work/frontier/w10a/tools/gt_remote.sh watchman2:rush2049/scratch/frontier/w10a/
scp -q "$src" watchman2:rush2049/scratch/frontier/w10a/cand/_t_$lab.c
ssh watchman2 "cd ~/rush2049/scratch/frontier/w10a && sh gt_remote.sh cand/_t_$lab.c $K $lab $proc"
scp -q watchman2:rush2049/scratch/frontier/w10a/st_$lab/list watchman2:rush2049/scratch/frontier/w10a/st_$lab/w5d watchman2:rush2049/scratch/frontier/w10a/st_$lab/cdx $OUT/
python3 $R/cloud/work/frontier/w5d/tools/prereport.py $OUT $proc > $OUT/report.txt
cp "$src" $OUT/source.c
echo "report: $OUT/report.txt"
