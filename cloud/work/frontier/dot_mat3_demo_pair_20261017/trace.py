"""Observation-only complete-object trace; raw logs stay in the requested output."""
from pathlib import Path
import argparse,hashlib,importlib.util,json,os,sys
HERE=Path(__file__).resolve().parent
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--repo',required=True,type=Path);p.add_argument('--uopt',required=True,type=Path);p.add_argument('--out',required=True,type=Path);a=p.parse_args();sys.path.insert(0,str(a.repo/'tools/cloud'));import score
s=importlib.util.spec_from_file_location('demo_verify',HERE/'verify.py');v=importlib.util.module_from_spec(s);s.loader.exec_module(v);a.out.mkdir(parents=True,exist_ok=True)
score.compile_group(HERE/'group',a.out/'stock.o');original=score._run

def observed(command,**kwargs):
 command=list(command)
 if Path(command[0]).name=='uopt':
  command[0]=str(a.uopt);env={k:w for k,w in os.environ.items() if not (k.startswith('CDX_') or k.startswith('W5D_'))};env.update(CDX_LOG='1',CDX_DETAIL_WEB='all',CDX_SYMTAB='1',CDX_OUT=str(a.out/'color.log'),W5D_PROC='debug_stats',W5D_OUT=str(a.out/'before.log'),W5D_LEVEL='3');kwargs['env']=env;command+=['-l',str(a.out/'uopt.list')]
 return original(command,**kwargs)
score._run=observed
try:score.compile_group(HERE/'group',a.out/'traced.o')
finally:score._run=original
v.require((a.out/'stock.o').read_bytes()==(a.out/'traced.o').read_bytes(),'trace changed whole object')
r={'observation_only':True,'forced_colors':False,'complete_objects_identical':True,'object_sha256':v.digest(a.out/'stock.o'),'inputs_sha256':v.inputs(),'traced_uopt_sha256':v.digest(a.uopt)}
(HERE/'trace_fidelity.json').write_text(json.dumps(r,indent=2)+'\n');print(r['object_sha256'])
