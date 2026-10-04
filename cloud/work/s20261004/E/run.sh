#!/bin/sh
# run.sh NAME KEEP MEMBERS [flags]
cd /home/cburnes/projects/rush2049-decomp/cloud/work/s20261004/E
N=$1; K=$2; M=$3; shift 3
ssh watchman2 "rm -f rush2049/scratch/s20261004_E/${N}_?.c"
scp -q build.sh $N.c $(ls ${N}_?.c 2>/dev/null) $K watchman2:rush2049/scratch/s20261004_E/ && ssh watchman2 "sh rush2049/scratch/s20261004_E/build.sh $N $K $*" && scp -q watchman2:rush2049/scratch/s20261004_E/$N.o obj/ && python3 verify.py obj/$N.o $M | python3 -c "
import json,sys; d=json.load(sys.stdin)
for r in d['results']: print(r['name'], r.get('symbol_bytes'), '/', r['slot_bytes'], 'IDENTICAL' if r.get('identical') else r.get('word_differences', r.get('refusal')))"
