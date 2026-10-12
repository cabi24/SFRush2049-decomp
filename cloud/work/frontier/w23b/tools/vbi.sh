#!/bin/sh
# w23b (Pi): ins.sh for every .c in DIR (lane-relative); run from the lane dir
d=$1; fn=$2
ssh watchman2 "mkdir -p rush2049/scratch/frontier/w23b/lane/$d"; rsync -a --delete $d/ watchman2:rush2049/scratch/frontier/w23b/lane/$d/
rsync -a tools/ watchman2:rush2049/scratch/frontier/w23b/lane/tools/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w23b && for f in lane/$d/*.c; do printf '%s ' \$(basename \$f); sh lane/tools/ins.sh \$f $fn; done" | sort -t= -k2 -n
