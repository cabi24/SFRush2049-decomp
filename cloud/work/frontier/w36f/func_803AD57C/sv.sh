#!/bin/bash
# sv.sh NAME 'ROOTBODY' : replace animText root body lines 749-755 region with given text; score
N=$1; B=$2
mkdir -p s/$N
python3 - "$N" "$B" <<'PY'
import sys
n,b=sys.argv[1],sys.argv[2]
g=open('n3/group.c').read()
old="""    render_helper(0.0f);
    switch(D_803BA804) {
    case 0:func_803AA3D8();break;
    case 1:drawView();break;
    }
    render_helper(-1.0f);
    return 1;
"""
assert old in g
g=g.replace(old,b)
open('s/'+n+'/group.c','w').write(g)
PY
cp n3/group.json s/$N/
echo -n "$N: "; ../tools/gd.sh group a s/$N func_803AD57C 2>&1 | tail -1
