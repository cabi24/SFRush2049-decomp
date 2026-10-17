"""Reproduce observation-only stock/traced IDO object identity; logs stay private."""
from pathlib import Path
import argparse,hashlib,json,os,sys,tempfile,importlib.util
HERE=Path(__file__).resolve().parent
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--repo',type=Path,required=True);p.add_argument('--trace-uopt',type=Path,required=True);p.add_argument('--out',type=Path);p.add_argument('--record',action='store_true');a=p.parse_args()
sys.path.insert(0,str(a.repo/'tools/cloud'));import score
s=importlib.util.spec_from_file_location('createbg_verify',HERE/'verify.py');v=importlib.util.module_from_spec(s);s.loader.exec_module(v)
out=a.out or Path(tempfile.mkdtemp(prefix='createbg-trace-'));out=out.resolve();out.mkdir(parents=True,exist_ok=True)
score.compile_group(HERE/'group',out/'stock.o');run=score._run

def observed(command,**kw):
    command=list(command)
    if Path(command[0]).name=='uopt':
        command[0]=str(a.trace_uopt.resolve());command+=['-l',str(out/'uopt.list')]
        env={k:x for k,x in os.environ.items() if not k.startswith(('CDX_','W5D_'))}
        env.update(CDX_LOG='1',CDX_DETAIL_WEB='all',CDX_SYMTAB='1',CDX_OUT=str(out/'colors.log'),W5D_PROC='particle_lifetime_set',W5D_OUT=str(out/'before.log'),W5D_LEVEL='3');kw['env']=env
    return run(command,**kw)
score._run=observed
try:score.compile_group(HERE/'group',out/'traced.o')
finally:score._run=run
v.require((out/'stock.o').read_bytes()==(out/'traced.o').read_bytes(),'trace changed object')
v.require((out/'uopt.list').stat().st_size>0 and (out/'colors.log').stat().st_size>0,'missing observation logs')
r={'observation_only':True,'forced_colors':False,'complete_objects_identical':True,'object_sha256':v.digest(out/'stock.o'),'inputs_sha256':v.inputs(),'trace_uopt_sha256':v.digest(a.trace_uopt)}
if a.record:(HERE/'trace_fidelity.json').write_text(json.dumps(r,indent=2)+'\n')
else:v.require(r==json.loads((HERE/'trace_fidelity.json').read_text()),'trace receipt changed')
print('PASS: complete stock/traced object identity')
