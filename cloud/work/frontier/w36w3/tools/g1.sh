#!/bin/bash
# usage: g1.sh FILE.c FUNC [KEEP,LIST] [gscore args]  -> one-file -O3 group (keep list default FUNC) + aligned diff of FUNC
F=$1; N=$2; shift 2; K=$N; if [ -n "$1" ] && [ "${1#--}" = "$1" ]; then K=$1; shift; fi
D=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad/g1_$N
rm -rf $D; mkdir -p $D; cp $F $D/one.c
python3 - "$D" "$N" "$K" <<'PY'
import json, sys
d, n, k = sys.argv[1:4]
json.dump({"members": [n], "files": ["one.c"], "keep": k.split(','), "flags": "-g0 -O3 -mips2 -G 0 -non_shared", "context": []}, open(d + '/group.json', 'w'), indent=1)
PY
/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36w3/tools/gg.sh $D --names $N --diff $N "$@"
