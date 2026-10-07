#!/bin/sh
# ora.sh BASE.s OUTNAME EDIT... : listing oracle. EDIT = dN (delete line N) | iN:TEXT (insert TEXT before line N; \t ok)
# Line numbers refer to BASE.s. Reassembles with stock as0/as1 (toolkit asm.sh, label base) and prints the E05F0 row summary.
B=$1; O=$2; shift 2
D=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w13b/ora; mkdir -p $D
python3 - "$B" "$D/$O.s" "$@" <<'PY'
import sys
src=open(sys.argv[1]).read().split('\n'); out=sys.argv[2]; dels=set(); ins={}
for e in sys.argv[3:]:
    if e[0]=='d': dels.add(int(e[1:]))
    else:
        n,t=e[1:].split(':',1); ins.setdefault(int(n),[]).append('\t'+t.replace('\\t','\t'))
res=[]
for i,l in enumerate(src,1):
    res+=ins.get(i,[])
    if i not in dels: res.append(l)
open(out,'w').write('\n'.join(res))
PY
cd /home/cburnes/projects/rush2049-decomp && TAG=w13b sh cloud/work/frontier/tools/trace/asm.sh base $D/$O.s func_800E05F0 --summary 2>&1 | grep -E "differing|want" | tr '\n' ' '; echo
