#!/bin/sh
# w19a (Pi): score every .c in DIR for FN (LOCK line only)
d=$1; fn=$2
ssh watchman2 "mkdir -p rush2049/scratch/frontier/w19a/lane/$d"; rsync -a --delete /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w19a/$d/ watchman2:rush2049/scratch/frontier/w19a/lane/$d/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w19a && for f in lane/$d/*.c; do printf '%s ' \$(basename \$f); sh lane/tools/sc.sh \$f $fn 2>&1 | grep -E '^LOCK:' ; done"
