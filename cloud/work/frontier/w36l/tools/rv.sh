#!/bin/bash
# usage: rv.sh BASEDIR NAME 'python-edit-code operating on s (rocket.c text)' [FILE]
B=$1; N=$2; CODE=$3; F=${4:-rocket.c}
D=$(dirname $B)/$N
rm -rf $D; cp -r $B $D
python3 - "$D/$F" <<PY
import sys
p=sys.argv[1]; s=open(p).read(); o=s
$CODE
if s==o: print("WARNING: no change")
open(p,'w').write(s)
PY
/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36l/tools/g.sh $D --targets asm/us/blob --names ${NAMES:-entity_tick_main,drone_ai_update,matrix_scale_apply} 2>&1 | grep -v '^compiled functions'
