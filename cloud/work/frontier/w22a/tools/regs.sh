#!/bin/sh
# w22a (Pi): for each .c in lane-relative DIR, compile FN and print selected disassembly lines (grep -E PAT) in one row
d=$1; fn=$2; pat=$3
ssh watchman2 "mkdir -p rush2049/scratch/frontier/w22a/lane/$d"; rsync -a --delete /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w22a/$d/ watchman2:rush2049/scratch/frontier/w22a/lane/$d/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w22a && for f in lane/$d/*.c; do printf '%s ' \$(basename \$f); sh lane/tools/obj.sh \$f $fn lane/r.o >/dev/null 2>&1 && mips-linux-gnu-objdump -d lane/r.o | grep -E '$pat' | awk '{\$1=\"\";\$2=\"\"; print}' | tr '\n' ';'; echo; done"
