#!/bin/sh
scp -q "$1" watchman2:rush2049/scratch/frontier/w2d/cand/_x.s && ssh watchman2 "cd ~/rush2049/scratch/frontier/w2d/cand && sh asx_remote.sh _x.s"
