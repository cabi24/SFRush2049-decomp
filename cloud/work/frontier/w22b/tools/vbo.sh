#!/bin/sh
# w22b (Pi): ops.sh for every .c in DIR (under the lane dir); run from the lane dir
d=$1; fn=$2
ssh watchman2 "mkdir -p rush2049/scratch/frontier/w22b/lane/$d"; rsync -a --delete $d/ watchman2:rush2049/scratch/frontier/w22b/lane/$d/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w22b && for f in lane/$d/*.c; do printf '%s ' \$(basename \$f); sh lane/tools/ops.sh \$f $fn; done"
