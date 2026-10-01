import json,subprocess,shlex,sys,os,hashlib
from pathlib import Path
sys.path.insert(0,'tools/cloud');import score
os.environ['CONVEYOR_TOOLKIT']=str(Path.home()/'rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5')
sys.path.insert(0,'tools/conveyor/jobs');import scoring
p=Path.home()/'agents/A/scratch/codex_A11';out=[]
for r in json.load(open(p/'manifest.json')):
 n=r['function'];r=dict(r);r['flags']='-g0 -O1 -mips2 -G 0 -non_shared';obj=p/'targets'/(n+'.O1.o');h=p/'headers';proc=subprocess.run([score.ido('cc'),'-c',*shlex.split(r['flags']),'-I'+str(h),'-I'+str(h/'include'),'-I'+str(h/'include/PR'),'-D_LANGUAGE_C',str(p/'sources'/(n+'.c')),'-o',str(obj)],capture_output=True,text=True);r.update(exit=proc.returncode,stderr=proc.stderr)
 if proc.returncode==0:
  target=p/'targets'/(n+'.target.o');a=score.text_words(target);b=score.text_words(obj);r.update(strict_score=scoring.score(target,obj,stack_differences=True),raw_word_diff=sum(v!=w for v,w in zip(a,b))+abs(len(a)-len(b)),target_words=len(a),candidate_words=len(b),candidate_o_sha256=hashlib.sha256(obj.read_bytes()).hexdigest())
 print(n,r,flush=True);out.append(r)
(p/'results.json').write_text(json.dumps(out,indent=2)+'\n')
