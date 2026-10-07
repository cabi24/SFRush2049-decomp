"""Uniform host-C checks of frozen camera_transform proposals, not native proof."""
import argparse,hashlib,json,re,subprocess,tempfile,time,datetime
from pathlib import Path

def call(args, **kw):
 p=subprocess.run(args,capture_output=True,text=True,timeout=30,**kw)
 if p.returncode:raise RuntimeError(json.dumps({'command':args,'returncode':p.returncode,'stdout':p.stdout[:3000],'stderr':p.stderr[:3000]}))
 return p

def build_run(base, source, work, harness):
 work.mkdir(parents=True,exist_ok=True)
 for name,path,prefix in [('baseline',base,'baseline'),('candidate',source,'candidate')]:
  call(['gcc','-std=c89','-pedantic-errors','-O0','-fno-strict-aliasing','-Dcamera_transform='+prefix+'_camera_transform','-DMP_TargetSteerPos='+prefix+'_MP_TargetSteerPos','-c',str(path),'-o',str(work/(name+'.o'))])
 call(['gcc','-std=c99','-O0',str(harness),str(work/'baseline.o'),str(work/'candidate.o'),'-o',str(work/'check')])
 p=subprocess.run([str(work/'check')],capture_output=True,text=True,timeout=15)
 result=json.loads(p.stdout)
 return result

def verify(root,out):
 start=time.monotonic();utc=datetime.datetime.now(datetime.timezone.utc).isoformat();root=Path(root).resolve();out=Path(out).resolve();out.mkdir(parents=True,exist_ok=True)
 base=root/'baseline.c';plan=json.loads((root/'batch.json').read_text());harness=Path(__file__).with_name('semantic_harness.c');rows=[]
 for key,p in sorted(plan['predictions'].items()):
  path=root/p['source'];assert hashlib.sha256(path.read_bytes()).hexdigest()==p['sha256'];s=time.monotonic();r=build_run(base,path,out/key,harness);rows.append({'id':key,'sha256':p['sha256'],**r,'elapsed_seconds':time.monotonic()-s})
 result={'status':'passed' if all(x['status']=='bounded_pass' for x in rows) else 'failed','cases_per_candidate':2048,'start_utc':utc,'end_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'elapsed_seconds':time.monotonic()-start,'rows':rows,'scope':'Host-C differential traces/state against frozen clean C, 2048 deterministic scenarios; standard 32-bit int/float but host pointers/layout, not O32 or whole-C/native equivalence. Covers real commands, list mutations, flags, retries, history and bounded callback-sensitive reloads.'}
 (out/'result.json').write_text(json.dumps(result,indent=2)+'\n');return result

if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('lane');p.add_argument('out');a=p.parse_args();print(json.dumps(verify(a.lane,a.out),indent=2))
