#!/usr/bin/env python3
"""mkv.py BASE_TAIL.c variants.py : V={name:[(old,new),...]} -> var/t_<name>.c ; then tv.sh each (ONLY= filter)"""
import sys,os,subprocess
W=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
base=open(sys.argv[1]).read(); ns={}; exec(open(sys.argv[2]).read(),ns)
for name,reps in ns['V'].items():
    if os.environ.get('ONLY') and name not in os.environ['ONLY'].split(','): continue
    s=base; ok=True
    for o,n in reps:
        if o not in s: print(name,'| MISSING',repr(o[:70])); ok=False; break
        s=s.replace(o,n,1)
    if not ok: continue
    p=os.path.join(W,'var','t_%s.c'%name); open(p,'w').write(s)
    subprocess.run([os.path.join(W,"tools",os.environ.get("TOOL","tv.sh")),p]+sys.argv[3:])
