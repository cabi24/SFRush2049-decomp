#!/usr/bin/env python3
"""fvar.py BASE.c VARIANTS.py FN [flags] -- builder single-file variants (fn mode); prints differing rows per variant"""
import sys, subprocess, os
base=open(sys.argv[1]).read(); ns={}; exec(open(sys.argv[2]).read(), ns)
fn=sys.argv[3]; flags=sys.argv[4] if len(sys.argv)>4 else '-g0 -O3 -mips2 -G 0 -non_shared'
d=os.path.dirname(sys.argv[1])
for name,subs in ns['V'].items():
    s=base; ok=True
    for o,n in subs:
        if o not in s: print(name,'MISSING',repr(o[:50])); ok=False; break
        s=s.replace(o,n)
    if not ok: continue
    p=os.path.join(d,'v_%s.c'%name); open(p,'w').write(s)
    r=subprocess.run(['cloud/work/frontier/w2d/full.sh',p,fn,'--all','--flags','"%s"'%flags],capture_output=True,text=True)
    open(os.path.join(d,'v_%s.txt'%name),'w').write(r.stdout+r.stderr)
    print(name,'|',(r.stdout.strip().splitlines() or [r.stderr[-300:]])[-1])
