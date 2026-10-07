#!/usr/bin/env python3
"""udump.py MERGED BLOCK [FROM_LINE TO_LINE] : compact dump of one procedure's ucode (Uent block number BLOCK)."""
import sys
sys.path.insert(0,'/home/cburnes/projects/rush2049-decomp/third_party/n64-decomp-workbench/src')
from decomp_workbench.ucode import parse_ucode
from pathlib import Path
recs=parse_ucode(Path(sys.argv[1])); blk=int(sys.argv[2])
lo=int(sys.argv[3]) if len(sys.argv)>3 else 0; hi=int(sys.argv[4]) if len(sys.argv)>4 else 10**9
on=False; line=0
for r in recs:
    d=r.as_dict()
    if d['name']=='Uent': on = int(d['words'][1],16)==blk
    if not on: continue
    if d['name']=='Uloc':
        line=int(d['words'][1],16); continue
    if lo<=line<=hi:
        det=d.get('detail','')
        if d['name']=='Uldc' and 'constant_value' in d: det='%s %s'%(d['dtype_name'],d['constant_value'])
        print('%5d L%-4d %-6s %s'%(d['index'],line,d['name'][1:],det))
    if d['name']=='Uend': on=False
