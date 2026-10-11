#!/bin/sh
# w21c (Pi): count non-branch diff rows for every .c in lane-relative DIR
d=$1; fn=$2
rsync -a --delete /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w21c/$d/ watchman2:rush2049/scratch/frontier/w21c/lane/$d/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w21c && for f in lane/$d/*.c; do n=\$(sh lane/tools/dif.sh \$f $fn | grep -P '\s[|<>](\s|\$)' | grep -vP '\t(b|beq|bne|beqz|bnez|beqzl|bnezl|blezl|bnel|beql|bgez|bc1fl|bc1tl|bc1f|bc1t)\s' | grep -v 'b[a-z0-9]* .*<func.*b[a-z0-9]* .*<func' | wc -l); echo \"\$n \$(basename \$f)\"; done" | sort -n | head -${N:-12}
