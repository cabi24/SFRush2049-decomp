#!/usr/bin/env python3
"""mk.py BASE.c OUT.c OLD NEW [OLD NEW...]: exact-substring substitution (each OLD must occur once)."""
import sys
s=open(sys.argv[1]).read(); a=sys.argv[3:]
for o,n in zip(a[::2],a[1::2]):
    o=o.replace('\\n','\n'); n=n.replace('\\n','\n')
    assert s.count(o)==1,(o,s.count(o)); s=s.replace(o,n)
open(sys.argv[2],'w').write(s)
