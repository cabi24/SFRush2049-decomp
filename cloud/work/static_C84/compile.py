import sys,os,subprocess,json,hashlib
from pathlib import Path
p=Path.home()/'agents/C/scratch/static-C84';tk=Path.home()/'rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5';wt=Path.home()/'agents/C/wt';os.environ['CONVEYOR_TOOLKIT']=str(tk);sys.path.insert(0,str(wt/'tools/conveyor/jobs'));import scoring
out=[]
for r in json.loads((p/'inputs.json').read_text()):
 src=p/(r['variant']+'.c');obj=p/(r['variant']+'.o');cmd=[str(tk/'ido/cc'),'-c',*r['flags'].split(),'-I'+str(Path.home()/'agents/C/scratch/static-C67/module/include'),'-I'+str(Path.home()/'agents/C/scratch/static-C67/module/rom'),str(src),'-o',str(obj)];cp=subprocess.run(cmd,capture_output=True,text=True);r.update(compile_exit=cp.returncode,stderr=cp.stderr)
 if not cp.returncode:r.update(strict_score=scoring.score(p/(r['function']+'.target.o'),obj,stack_differences=True),object_sha256=hashlib.sha256(obj.read_bytes()).hexdigest())
 print(json.dumps(r),flush=True);out.append(r)
(p/'results.json').write_text(json.dumps(out,indent=2)+'\n')
