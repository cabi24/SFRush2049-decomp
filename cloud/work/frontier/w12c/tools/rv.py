#!/usr/bin/env python3
"""rv.py BASEFILE FUNC variants.py [FUNC2...]: variants.py defines V = {name: [(old,new),...]}.
Each variant applies its replacements to BASEFILE (one of comp/*.c), scores the whole w12c component
(all other comp files fixed) and prints the unit lines for the FUNCs plus udiff words/ops summary."""
import sys, os, subprocess
R='/home/cburnes/projects/rush2049-decomp'
C=R+'/cloud/work/frontier/w12c/comp'
base=os.path.abspath(sys.argv[1]); funcs=[sys.argv[2]]+sys.argv[4:]
ns={}; exec(open(sys.argv[3]).read(), ns)
files=['d5e64.c','mode.c','msh.c','ded78.c']
INT=['best_times_display','mode_select_input','func_800DFBA0','mode_select_handler','func_800DED78','func_800E0050']
tag=os.environ.get('TAG','w12c')
src=open(base).read()
out=os.path.join(os.path.dirname(os.path.abspath(sys.argv[3])))
for name, reps in ns['V'].items():
    if os.environ.get('ONLY') and name not in os.environ['ONLY'].split(','): continue
    s=src
    ok=True
    for o,n in reps:
        if o not in s: print(name,'| MISSING',repr(o[:60])); ok=False; break
        s=s.replace(o,n,1)
    if not ok: continue
    p=os.path.join(out,'v_%s_%s'%(name,os.path.basename(base)))
    open(p,'w').write(s)
    args=['python3','-m','tools.conveyor.pipeline.blob_unit','--tag',tag,'score']+funcs
    for f in files:
        args+=['--with', p if (os.path.join(C,f)==base or os.environ.get('SLOT')==f) else os.path.join(C,f)]
    for i in INT: args+=['--internal',i]
    r=subprocess.run(args,cwd=R,capture_output=True,text=True)
    lines=[l.strip() for l in r.stdout.splitlines() if any((' %s:'%f) in l for f in funcs) and (l.strip().startswith('FAIL') or l.strip().startswith('EQUAL'))]
    if not lines: print(name,'| ERR',(r.stdout[-400:]+r.stderr[-600:]).replace('\n',' ')); continue
    summ=[]
    for f in funcs:
        a=subprocess.run(['python3','cloud/work/frontier/tools/trace/udiff.py',f,'--summary'],cwd=R,capture_output=True,text=True,env=dict(os.environ,TAG=tag)).stdout.strip().splitlines()
        o=subprocess.run(['python3','cloud/work/frontier/tools/trace/udiff.py',f,'--ops','--summary'],cwd=R,capture_output=True,text=True,env=dict(os.environ,TAG=tag)).stdout.strip().splitlines()
        g=lambda x: x[-1].split(';')[1].strip() if x and ';' in x[-1] else '?'
        summ.append('%s w[%s] o[%s]'%(f[-6:],g(a),g(o)))
    print(name,'|',' ; '.join(summ),'|',' || '.join(lines),flush=True)
