#!/bin/sh
# sp.sh SRC.c LABEL KEEP -- area / spill-temp trace; log in runs/LABEL/sp.log
R=/home/cburnes/projects/rush2049-decomp
src=$1; lab=$2; K=$3
OUT=$R/cloud/work/frontier/w9e/runs/$lab; mkdir -p $OUT
scp -q $R/cloud/work/frontier/w9e/tools/sp_remote.sh watchman2:rush2049/scratch/frontier/w9e/
scp -q "$src" watchman2:rush2049/scratch/frontier/w9e/cand/_s_$lab.c
ssh watchman2 "cd ~/rush2049/scratch/frontier/w9e && sh sp_remote.sh cand/_s_$lab.c $K $lab"
scp -q watchman2:rush2049/scratch/frontier/w9e/st_$lab/sp.log $OUT/sp.log
