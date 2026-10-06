#!/usr/bin/env python3
"""svar.py FUNC BASE.c variants.py [blob_unit args] -- variants.py defines OLD and V{name: replacement for OLD}."""
import sys, subprocess, os
func, base, vfile = sys.argv[1:4]; extra=sys.argv[4:]
R0='/home/cburnes/projects/rush2049-decomp'
src=open(base).read(); ns={}; exec(open(vfile).read(), ns)
assert ns['OLD'] in src
od=os.path.join(os.path.dirname(os.path.abspath(vfile)),'out'); os.makedirs(od,exist_ok=True)
only=os.environ.get('ONLY')
for k,v in ns['V'].items():
    if only and k not in only.split(','): continue
    t=src.replace(ns['OLD'],v)
    if 'PRE' in ns: t=t.replace(ns['PRE_OLD'],ns['PRE']+ns['PRE_OLD'],1)
    for a,b in ns.get('POSTMAP',{}).get(k,[]):
        assert a in t, a
        t=t.replace(a,b)
    p=os.path.join(od,k+'.c'); open(p,'w').write(t)
    r=subprocess.run(['python3','-m','tools.conveyor.pipeline.blob_unit','--tag','w12b','score',func,'--with',p]+extra,cwd=R0,capture_output=True,text=True)
    line=[l.strip() for l in r.stdout.splitlines() if (' %s:'%func) in l]
    a=subprocess.run(['python3','cloud/work/frontier/tools/trace/udiff.py',func,'--summary'],cwd=R0,capture_output=True,text=True,env=dict(os.environ,TAG='w12b')).stdout.strip().splitlines()
    print(k,'|',a[-1].split(';')[1] if a and ';' in a[-1] else '?','|',line[0] if line else (r.stdout+r.stderr)[-300:].replace('\n',' / '),flush=True)
