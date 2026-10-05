#!/bin/sh
scp -q "$1" watchman2:rush2049/scratch/frontier/w2j/cand/_s.c && ssh watchman2 "cd ~/rush2049/scratch/frontier/w2j/cand && sh o3s_remote.sh _s.c render_large_objects"
