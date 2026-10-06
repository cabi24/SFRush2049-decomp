#!/bin/bash
# usage: fd.sh body.c [--all] : build group.c from body, aligned diff of steering_sensitivity
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w9a
G=$W/groups/steering_sensitivity
cat $W/pre.c $1 $W/post.c > $G/group.c
shift
scp -q $G/group.c watchman2:rush2049/scratch/frontier/w9a/cand/_g.c
ssh watchman2 "cd ~/rush2049/scratch/frontier/w9a && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido taskset -c 0,1 python3 cand/full.py cand/_g.c steering_sensitivity --flags '-g0 -O3 -mips2 -G 0 -non_shared' --keep steering_sensitivity,traction_control,func_800A61B0,math_utility $*"
