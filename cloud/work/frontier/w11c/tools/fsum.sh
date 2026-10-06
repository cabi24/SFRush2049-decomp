#!/bin/sh
# fsum.sh LABEL PROC FORCESPEC: traced run with forcing on snapshot, print colour summary
ssh watchman2 "cd ~/rush2049/scratch/frontier/w11c && ./tr.sh st_$1 \$PWD/st_$1/f.txt CDX_PROC=$2 CDX_DETAIL_WEB=all CDX_FORCE='$3' && ./sum.sh st_$1/f.txt"
