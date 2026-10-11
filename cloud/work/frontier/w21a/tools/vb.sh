#!/bin/sh
# w21a (Pi): score every .c in lane-relative DIR for FN (LOCK line only), sorted
d=$1; fn=$2
ssh watchman2 "mkdir -p rush2049/scratch/frontier/w21a/lane/$d"; rsync -a --delete /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w21a/$d/ watchman2:rush2049/scratch/frontier/w21a/lane/$d/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w21a && for f in lane/$d/*.c; do printf '%s ' \$(basename \$f); sh lane/tools/sc.sh \$f $fn 2>&1 | grep -E '^LOCK:' ; done" | sort -t= -k2 -n | head -${N:-15}
