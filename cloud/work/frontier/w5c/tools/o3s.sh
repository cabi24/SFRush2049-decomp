#!/bin/sh
# usage: o3s.sh file.c NAME -- ugen listing (pre-as1), -O3 whole-program pipeline
scp -q "$1" watchman2:rush2049/scratch/frontier/w5c/cand/_s.c && ssh watchman2 "cd ~/rush2049/scratch/frontier/w5c/cand && sh o3s_remote.sh _s.c $2"
