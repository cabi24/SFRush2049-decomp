#!/bin/sh
# w21b (Pi): score every .c in DIR (under the lane dir) for FN (LOCK line only); run from the lane dir
d=$1; fn=$2
ssh watchman2 "mkdir -p rush2049/scratch/frontier/w21b/lane/$d"; rsync -a --delete $d/ watchman2:rush2049/scratch/frontier/w21b/lane/$d/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w21b && for f in lane/$d/*.c; do printf '%s ' \$(basename \$f); sh lane/tools/sc.sh \$f $fn 2>&1 | grep -E '^LOCK:' ; done"
