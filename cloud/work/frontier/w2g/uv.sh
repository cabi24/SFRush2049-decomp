#!/bin/sh
# usage: uv.sh file.c keep,list -- umerge -v inlining log on the builder
scp -q "$1" watchman2:rush2049/scratch/frontier/w2g/cand/_ex.c && ssh watchman2 "cd ~/rush2049/scratch/frontier/w2g/cand && sh o3v_remote.sh _ex.c $2"
