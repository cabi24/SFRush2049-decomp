import json,os,sys,subprocess,shlex,hashlib
from pathlib import Path
wt=Path.home()/'agents/C/wt'; sys.path[:0]=[str(wt/'tools/conveyor/jobs'),str(wt/'tools/cloud')]
tk=Path.home()/'rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5';os.environ['CONVEYOR_TOOLKIT']=str(tk)
import scoring
import score as cloudscore
p=Path.home()/'agents/C/scratch/static-C22';results=[]
for src in sorted(p.glob('*.c')):
 n=src.stem
 for opt in ['O2','O1']:
  flags='-g0 -'+opt+' -mips2 -G 0 -non_shared'+' -Wab,-r4300_mul'
  obj=p/(n+'.'+opt+'.o');proc=subprocess.run([str(tk/'ido/cc'),'-c']+shlex.split(flags)+['-I'+str(p/'include'),'-I'+str(p),str(src),'-o',str(obj)],capture_output=True,text=True)
  if proc.returncode:r={'function':n,'flags':flags,'compile_error':proc.stderr[-2000:]}
  else:
   t=p/(n+'.target.o');strict=scoring.score(t,obj,stack_differences=True);a=cloudscore.text_words(t);b=cloudscore.text_words(obj)
   r={'function':n,'flags':flags,'strict_score':strict,'target_words':len(a),'candidate_words':len(b),'raw_word_diff':sum(x!=y for x,y in zip(a,b))+abs(len(a)-len(b)),'source_sha256':hashlib.sha256(src.read_bytes()).hexdigest(),'target_sha256':hashlib.sha256(t.read_bytes()).hexdigest()}
  print(json.dumps(r),flush=True);results.append(r)
(p/'results.json').write_text(json.dumps(results,indent=2)+'\n')
