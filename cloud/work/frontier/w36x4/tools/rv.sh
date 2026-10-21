#!/bin/bash
# usage: rv.sh BASEDIR NEWDIR 'python-edit-code operating on s (FILE text)' [FILE]   (NAMES env = functions to score)
B=$1; D=$2; CODE=$3; F=${4:-blast.c}
rm -rf $D; cp -r $B $D
python3 - "$D/$F" <<PY
import sys
p=sys.argv[1]; s=open(p).read(); o=s
def R(a,b,n=1):
    global s
    if a not in s: print("WARNING: not found:", a[:60]); return
    s=s.replace(a,b,n)
$CODE
if s==o: print("WARNING: no change")
open(p,'w').write(s)
PY
/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36x4/tools/g.sh $D --targets asm/us/blob --names ${NAMES:-func_80090308} $EXTRA 2>&1 | grep -v '^compiled functions'
