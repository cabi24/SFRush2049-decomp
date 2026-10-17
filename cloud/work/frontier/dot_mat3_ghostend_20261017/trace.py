"""Observation-only trace fidelity; raw outputs stay in an explicitly supplied directory."""
from pathlib import Path
import argparse,hashlib,importlib.util,json,os,sys
HERE=Path(__file__).resolve().parent

def run(repo,out,tracer):
 repo=Path(repo).resolve();out=Path(out).resolve();out.mkdir(parents=True,exist_ok=True);tracer=Path(tracer).resolve();sys.path.insert(0,str(repo/'tools/cloud'));import score
 original=score._run;score.compile_group(HERE/'group',out/'stock.o')
 def stage(command,**kw):
  command=list(command);name=Path(command[0]).name
  if name=='uopt':
   command[0]=str(tracer);env={k:v for k,v in os.environ.items() if not(k.startswith('CDX_') or k.startswith('W5D_'))};env.update(CDX_LOG='1',CDX_DETAIL_WEB='all',CDX_SYMTAB='1',CDX_OUT=str(out/'color.log'),W5D_PROC='ghost_race_setup',W5D_OUT=str(out/'pre.log'),W5D_LEVEL='3');kw['env']=env;command+=['-l',str(out/'uopt.list')]
  result=original(command,**kw);(out/(name+'.stderr')).write_text(result.stderr);return result
 score._run=stage
 try:score.compile_group(HERE/'group',out/'trace.o')
 finally:score._run=original
 if (out/'stock.o').read_bytes()!=(out/'trace.o').read_bytes():raise RuntimeError('trace changed object')
 s=importlib.util.spec_from_file_location('ghostend_verify',HERE/'verify.py');v=importlib.util.module_from_spec(s);s.loader.exec_module(v)
 result={'observation_only':True,'forced_colors':False,'complete_objects_identical':True,'object_sha256':hashlib.sha256((out/'stock.o').read_bytes()).hexdigest(),'inputs_sha256':v.inputs(),'traced_uopt_sha256':hashlib.sha256(tracer.read_bytes()).hexdigest()}
 (HERE/'trace_fidelity.json').write_text(json.dumps(result,indent=2)+'\n');return result
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--repo',required=True,type=Path);p.add_argument('--out',required=True,type=Path);p.add_argument('--traced-uopt',required=True,type=Path);a=p.parse_args();r=run(a.repo,a.out,a.traced_uopt);print(r['object_sha256'])
