#!/bin/sh
# w23c (Pi, repo root): LOCK line (check.py) for every .c in lane DIR vs tgt/FN.o
d=$1; fn=$2
rsync -a cloud/work/frontier/w23c/ watchman2:rush2049/scratch/frontier/w23c/lane/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w23c && for f in lane/$d/*.c; do printf '%-24s ' \$(basename \$f); sh lane/tools/sc.sh \$f $fn 2>&1 | grep -E '^LOCK:' | sed 's/objdump-dr //'; done"
