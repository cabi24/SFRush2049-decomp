#!/bin/sh
# mk.sh BODY.c OUTDIR : splice BODY (a func_803A9088 definition) into a copy of the installed group.
cd /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w39b/A9088
mkdir -p "$2" && cp base/group.json "$2"/ && python3 - "$1" "$2" <<'PY'
import sys
s=open('base/group.c').read()
start=s.index('void func_803A9088(s32 player)\n{')
end=s.index('void func_803A7B58(s32);\nvoid func_803A87D4(s32);')
b=open(sys.argv[1]).read()
open(sys.argv[2]+'/group.c','w').write(s[:start]+b+s[end:])
PY
