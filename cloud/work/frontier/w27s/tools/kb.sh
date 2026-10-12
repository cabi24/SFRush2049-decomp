#!/bin/sh
# Pi side: kb.sh FN FILE... -> builder kc.sh per variant (cc -K + traced ugen/as1), differing rows vs tgt
S=rush2049/scratch/frontier/w27s; fn=$1; shift
ssh watchman2 "mkdir -p ~/$S/kv/$fn" && scp -q "$@" watchman2:$S/kv/$fn/ || exit 1
scp -q $(dirname $0)/kc.sh watchman2:$S/lane/tools/
n=""; for f in "$@"; do n="$n $(basename $f .c)"; done
ssh watchman2 "cd ~/$S && for b in $n; do printf '%-12s ' \$b; sh lane/tools/kc.sh kv/$fn/\$b.c $fn \$PWD/kv/$fn/d_\$b 2>&1 | tail -1; done"
