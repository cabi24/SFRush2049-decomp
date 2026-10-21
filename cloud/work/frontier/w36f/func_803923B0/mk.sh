#!/bin/bash
# mk.sh SRC.c OUTDIR : build a group dir = g0/group.c head (lines 1..611, D_80140BDC volatile) + SRC.c; group.json with 923B0 claimed
S=$1; O=$2
mkdir -p $O
head -n 611 g0/group.c | sed 's/^extern u8 D_80140BDC;/extern volatile u8 D_80140BDC;/' > $O/group.c
cat $S >> $O/group.c
python3 - $O <<'PY'
import json,sys
j=json.load(open('g0/group.json'))
j['members']=j['members']+['func_803923B0']
j['context']=[c for c in j['context'] if c!='func_803923B0']
j['claims']=['func_803923B0']
json.dump(j,open(sys.argv[1]+'/group.json','w'),indent=1)
PY
