#!/bin/sh
# sp.sh SRC.c LABEL [KEEP] -- area / spill-temp trace of the group; log in runs/LABEL/sp.log
R=/home/cburnes/projects/rush2049-decomp
src=$1; lab=$2; K=${3:-camera_update_c,select_screen_update,Input_ProcessGameplayPad}
OUT=$R/cloud/work/frontier/w8b/runs/$lab; mkdir -p $OUT
scp -q $R/cloud/work/frontier/w8b/tools/sp_remote.sh watchman2:rush2049/scratch/frontier/w8b/
scp -q "$src" watchman2:rush2049/scratch/frontier/w8b/cand/_s_$lab.c
ssh watchman2 "cd ~/rush2049/scratch/frontier/w8b && sh sp_remote.sh cand/_s_$lab.c $K $lab" >/dev/null
scp -q watchman2:rush2049/scratch/frontier/w8b/st_$lab/sp.log $OUT/sp.log
