#!/usr/bin/env python3
"""cvar.py FUNC variants.py BASEFILE -- other --with files fixed; replace BASEFILE's FUNC def per variant; unit-score whole component."""
import sys, re, subprocess, os
func, vfile, base = sys.argv[1], sys.argv[2], sys.argv[3]
C='/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/comp'
others=[f for f in ['d5e64.c','mode.c','msh.c','ded78.c'] if os.path.join(C,f)!=os.path.abspath(base)]
src=open(base).read()
m=re.search(r'^[^\n;]*\b%s\([^;{]*\)\s*\{.*?^\}\n' % func, src, re.S|re.M)
assert m
ns={}
exec(open(vfile).read(), ns)
outdir=os.path.dirname(os.path.abspath(vfile))
INT=['best_times_display','mode_select_input','func_800DFBA0','mode_select_handler','func_800DED78']
for name, body in ns['V'].items():
    s=src[:m.start()]+body.strip('\n')+'\n'+src[m.end():]
    p=os.path.join(outdir,'cv_%s_%s'%(name,os.path.basename(base)))
    open(p,'w').write(s)
    args=['python3','-m','tools.conveyor.pipeline.blob_unit','--tag','w12b','score',func,'--with',p]
    for o in others: args+=['--with',os.path.join(C,o)]
    for i in INT: args+=['--internal',i]
    r=subprocess.run(args,cwd='/home/cburnes/projects/rush2049-decomp',capture_output=True,text=True)
    line=[l for l in r.stdout.splitlines() if (' %s:'%func) in l]
    a=subprocess.run(['python3','cloud/work/frontier/tools/trace/udiff.py',func,'--summary'],cwd='/home/cburnes/projects/rush2049-decomp',capture_output=True,text=True,env=dict(os.environ,TAG='w12b')).stdout.strip().splitlines()
    o=subprocess.run(['python3','cloud/work/frontier/tools/trace/udiff.py',func,'--ops','--summary'],cwd='/home/cburnes/projects/rush2049-decomp',capture_output=True,text=True,env=dict(os.environ,TAG='w12b')).stdout.strip().splitlines()
    def g(x):
        try: return x[-1].split(';')[1].strip()+' '+x[-1].split(';')[2].strip()
        except Exception: return '?'
    print(name,'|',g(a),'|',g(o).split(' frame')[0],'|',line[0].strip() if line else (r.stdout[-300:]+r.stderr[-300:]),flush=True)
