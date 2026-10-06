#!/bin/sh
# ugt.sh file.c NAME -- traced ugen output (free-list events then listing)
scp -q "$1" watchman2:rush2049/scratch/frontier/w11a/cand/_u.c && scp -q /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11a/tools/ugt_remote.sh watchman2:rush2049/scratch/frontier/w11a/cand/ && ssh watchman2 "cd ~/rush2049/scratch/frontier/w11a/cand && sh ugt_remote.sh _u.c $2"
