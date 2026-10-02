import os,sys,json,subprocess,hashlib
from pathlib import Path
p=Path.home()/'agents/C/scratch/static-C49';wt=Path.home()/'agents/C/wt';tk=Path.home()/'rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5'
os.environ['CONVEYOR_TOOLKIT']=str(tk);sys.path[:0]=[str(wt/'tools/conveyor/jobs'),str(wt/'tools/cloud')]
import scoring,score
out=[]
for n,addr in [('__osPiGetCmdQueue',0x800100c0),('osEPiRawWriteIo',0x8000fda0)]:
 for opt in ['O2','O1']:
  s=p/(n+'.c');o=p/(n+'.'+opt+'.o');flags=s.read_text().splitlines()[0][10:-3].replace('O1',opt).replace('O2',opt)
  r={'function':n,'flags':flags,'source_sha256':hashlib.sha256(s.read_bytes()).hexdigest()}
  cp=subprocess.run([str(tk/'ido/cc'),'-c',*flags.split(),str(s),'-o',str(o)],capture_output=True,text=True);r['compile_exit']=cp.returncode;r['stderr']=cp.stderr
  if cp.returncode:out.append(r);continue
  out.append(r);print(json.dumps(r))
(p/'results.json').write_text(json.dumps(out,indent=2)+'\n')
