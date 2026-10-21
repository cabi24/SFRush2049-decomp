#!/usr/bin/env python3
"""catlst.py OUT LIST1.s LIST2.s ... : concatenate ugen listings, renumbering local labels ($N, N>=32, and $$N) per input."""
import re,sys
out=[]
for k,f in enumerate(sys.argv[2:]):
    off=100000*(k+1)
    s=open(f).read()
    s=re.sub(r'\$\$(\d+)', lambda m: '$$%d'%(int(m.group(1))+off), s)
    s=re.sub(r'(?<![\$\w])\$(\d+)\b', lambda m: '$%d'%(int(m.group(1))+off) if int(m.group(1))>=32 else m.group(0), s)
    if k>0: s='\n'.join(l for l in s.split('\n') if not l.startswith('\t.verstamp') and not l.startswith('\t.option\tpic0'))
    out.append(s)
open(sys.argv[1],'w').write('\n'.join(out))
