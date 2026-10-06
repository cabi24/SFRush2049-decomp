#!/bin/sh
# usage: force.sh LABEL PROC SPEC [--all]  (after tr.sh LABEL) -- forced colouring, aligned diff
lab=$1; proc=$2; spec=$3; shift 3
ssh watchman2 "cd ~/rush2049/scratch/frontier/w9h && sh cand/gforce_b.sh cand/tr_$lab $proc '$spec' && python3 cand/objdiff.py cand/tr_$lab/f.o camera_scene_manager $*"
