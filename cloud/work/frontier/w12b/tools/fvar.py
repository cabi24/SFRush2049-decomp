#!/usr/bin/env python3
"""fvar.py FUNC variants.py BASEFILE [extra blob_unit args] -- variants.py defines V{name: text}; text replaces BASEFILE
from the line starting with MARK (default 'static s8 *rdy') through the end of FUNC's definition. Other comp files fixed."""
import sys, re, subprocess, os
func, vfile, base = sys.argv[1], sys.argv[2], sys.argv[3]
extra=sys.argv[4:]
R0='/home/cburnes/projects/rush2049-decomp'
C=R0+'/cloud/work/frontier/w12b/comp'
others=[f for f in ['d5e64.c','mode.c','msh.c','ded78.c'] if os.path.join(C,f)!=os.path.abspath(base)]
src=open(base).read()
ns={}
exec(open(vfile).read(), ns)
mark=ns.get('MARK','static s8 *rdy')
st=src.index(mark)
m=re.search(r'^[^\n;]*\b%s\([^;{]*\)\s*\{.*?^\}\n' % func, src[st:], re.S|re.M)
en=st+m.end()
outdir=os.path.dirname(os.path.abspath(vfile))
INT=['best_times_display','mode_select_input','func_800DFBA0','mode_select_handler','func_800DED78']
only=os.environ.get('ONLY')
for name, body in ns['V'].items():
    if only and name not in only.split(','): continue
    s=src[:st]+body.strip('\n')+'\n'+src[en:]
    p=os.path.join(outdir,'cv_%s.c'%name)
    open(p,'w').write(s)
    args=['python3','-m','tools.conveyor.pipeline.blob_unit','--tag','w12b','score',func,'--with',p]
    for o in others: args+=['--with',os.path.join(C,o)]
    for i in INT: args+=['--internal',i]
    args+=extra
    r=subprocess.run(args,cwd=R0,capture_output=True,text=True)
    line=[l for l in r.stdout.splitlines() if (' %s:'%func) in l]
    if not line:
        print(name,'| BUILD FAIL', (r.stdout+r.stderr)[-400:].replace('\n',' / '),flush=True); continue
    a=subprocess.run(['python3','cloud/work/frontier/tools/trace/udiff.py',func,'--summary'],cwd=R0,capture_output=True,text=True,env=dict(os.environ,TAG='w12b')).stdout.strip().splitlines()
    o=subprocess.run(['python3','cloud/work/frontier/tools/trace/udiff.py',func,'--ops','--summary'],cwd=R0,capture_output=True,text=True,env=dict(os.environ,TAG='w12b')).stdout.strip().splitlines()
    def g(x):
        try: return x[-1].split(';')[1].strip()+' '+x[-1].split(';')[2].strip()
        except Exception: return '?'
    print(name,'|',g(a),'|',g(o).split(' frame')[0],'|',line[0].strip(),flush=True)
