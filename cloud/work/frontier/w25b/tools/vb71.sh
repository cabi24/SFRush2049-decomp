#!/bin/sh
# Pi side: vb71.sh FN FILE... -> builder: plain 7.1 compile of each variant's reduced TU, differing objdump rows vs tgt/FN.o
S=rush2049/scratch/frontier/w25b; fn=$1; shift
ssh watchman2 "rm -rf ~/$S/t71/vb/$fn; mkdir -p ~/$S/t71/vb/$fn" && scp -q "$@" watchman2:$S/t71/vb/$fn/ || exit 1
scp -q /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace71/remote/tk71.sh watchman2:$S/t71/
ssh watchman2 "cd ~/$S && for f in t71/vb/$fn/*.c; do printf '%-14s ' \$(basename \$f); sh t71/tk71.sh ~/$S obj ~/$S/\$f $fn 2>&1 | grep -E 'rows:|rror' | sed 's/(objdump.*//' | head -2 | tr '\n' ' '; echo; done"
