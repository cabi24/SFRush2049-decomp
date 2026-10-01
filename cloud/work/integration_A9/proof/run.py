import json,subprocess,shlex,sys,os
from pathlib import Path
sys.path.insert(0,'tools/cloud');import score
p=Path.home()/'agents/A/scratch/codex_A9';rows=json.load(open(p/'verification.json'));out=[]
os.environ['CONVEYOR_TOOLKIT']=str(Path.home()/'rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5')
sys.path.insert(0,'tools/conveyor/jobs');import scoring
for r in rows:
 if r['integration_status']!='ready':continue
 n=r['function'];src=p/'proof'/(n+'.c');obj=p/'proof'/(n+'.o')
 q=subprocess.run([score.ido('cc'),'-c',*shlex.split(r['flags']),'-I'+str(p/'headers'),'-I'+str(p/'headers/include'),'-I'+str(p/'headers/include/PR'),'-D_LANGUAGE_C',str(src),'-o',str(obj)],capture_output=True,text=True)
 x={'function':n,'exit':q.returncode,'stderr':q.stderr}
 if q.returncode==0:
  target=Path.home()/'agents/C/scratch/static-C10'/(n+'.target.o');a=score.text_words(target);b=score.text_words(obj);x.update(raw_word_diff=sum(v!=w for v,w in zip(a,b))+abs(len(a)-len(b)),strict_score=scoring.score(target,obj,stack_differences=True),target_words=len(a),candidate_words=len(b))
 print(n,x.get('raw_word_diff'),x.get('strict_score'),q.returncode,q.stderr[-600:],flush=True);out.append(x)
(p/'results.json').write_text(json.dumps(out,indent=2)+'\n')
