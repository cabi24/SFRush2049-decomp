#!/usr/bin/env python3
"""mk.py BASE.c OUT.c 'old=>new' ... : string replacements applied inside func_800E05F0's body only (each must hit)."""
import sys
s=open(sys.argv[1]).read()
a=s.index("void func_800E05F0(ModelView *model)")
head,body=s[:a],s[a:]
for r in sys.argv[3:]:
    o,n=r.split("=>")
    o=o.replace("\\n","\n"); n=n.replace("\\n","\n")
    if o not in body: sys.exit("miss: "+o)
    body=body.replace(o,n)
open(sys.argv[2],'w').write(head+body)
