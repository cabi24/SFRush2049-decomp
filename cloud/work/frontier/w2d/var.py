#!/usr/bin/env python3
"""var.py BASE.c VARIANTS.py [FN] -- VARIANTS.py defines V = {name: [(old,new),...]} ; runs each in the unit, prints rows-differing"""
import sys, subprocess, os, re
base=open(sys.argv[1]).read(); ns={}; exec(open(sys.argv[2]).read(), ns)
fn = sys.argv[3] if len(sys.argv)>3 else 'AdjustSpeed'
extra = ns.get('EXTRA', '').split()
d=os.path.dirname(sys.argv[1])
for name,subs in ns['V'].items():
    s=base
    ok=True
    for o,n in subs:
        if o not in s: print(name,'MISSING',repr(o[:40])); ok=False; break
        s=s.replace(o,n)
    if not ok: continue
    p=os.path.join(d,'v_%s.c'%name); open(p,'w').write(s)
    r=subprocess.run(['python3','-m','tools.conveyor.pipeline.blob_unit','--tag','w2d','score',fn]+extra+['--with',p],capture_output=True,text=True)
    out=r.stdout+r.stderr
    m=re.search(r'(EQUAL|FAIL) %s.*'%fn,out)
    r2=subprocess.run(['python3','cloud/work/frontier/w2d/objdiff.py','build/blob_unit/w2d/unit.o',fn,'--all'],capture_output=True,text=True)
    open(os.path.join(d,'v_%s.txt'%name),'w').write(r2.stdout)
    fr=re.search(r'addiu sp,sp,-(\d+)\s*$', r2.stdout.splitlines()[0]) if r2.stdout else None
    print(name, '|', m.group(0) if m else out[-300:], '|', r2.stdout.splitlines()[-1] if r2.stdout else r2.stderr[-200:])
