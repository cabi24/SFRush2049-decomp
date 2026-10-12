#!/bin/sh
# w23b (Pi): score every .c in lane-relative DIR for FN (LOCK line only), sorted
d=$1; fn=$2
ssh watchman2 "mkdir -p rush2049/scratch/frontier/w23b/lane/$d"; rsync -a --delete /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w23b/$d/ watchman2:rush2049/scratch/frontier/w23b/lane/$d/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w23b && for f in lane/$d/*.c; do printf '%s ' \$(basename \$f); sh lane/tools/sc.sh \$f $fn 2>&1 | grep -E '^LOCK:' ; done" | sort -t= -k2,2n -k3,3n | head -${N:-15}
