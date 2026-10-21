#!/bin/bash
# mk.sh SRC.c OUTDIR : stats base group with func_803AA3D8's body replaced by SRC.c
S=$1; O=$2; mkdir -p $O
python3 - $S $O <<'PY'
import sys,re
src=open(sys.argv[1]).read()
g=open('../stats/base/group.c').read()
i=g.index('void func_803AA3D8(void)\n{')
j=g.index('\n}\n',i)+3
g=g[:i]+src.rstrip()+'\n'+g[j:]
open(sys.argv[2]+'/group.c','w').write(g)
PY
cp ../stats/base/group.json $O/group.json
