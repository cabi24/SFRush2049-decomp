from pathlib import Path
import os,sys,subprocess,json,hashlib
base=Path.home()/'agents/B/scratch/codex_B';tk=Path.home()/'rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5';repo=Path.home()/'agents/B/wt';os.environ['CONVEYOR_TOOLKIT']=str(tk);sys.path[:0]=[str(repo/'tools/conveyor/jobs'),str(repo/'tools/cloud')]
import scoring,score
rows=[]
for fn in ['osSetEventMesg_stackfield','vi_manager_main_local']:
 src=base/(fn+'.c');obj=base/(fn+'.o');p=subprocess.run([str(tk/'ido/cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared',str(src),'-o',str(obj)],capture_output=True,text=True);assert p.returncode==0,p.stderr
 targets=['osSetEventMesg','osSetEventMesg_stackalias'] if fn.startswith('osSet') else ['vi_manager_main']
 for tn in targets:
  target=base/(tn+'.target.o');t,c=score.text_words(target),score.text_words(obj);rows.append(dict(source=fn,source_sha256=hashlib.sha256(src.read_bytes()).hexdigest(),target=tn,target_sha256=hashlib.sha256(target.read_bytes()).hexdigest(),strict_score=scoring.score(target,obj,stack_differences=True),raw_word_diff=sum(x!=y for x,y in zip(t,c))+abs(len(t)-len(c)),target_words=len(t),candidate_words=len(c),flags='-g0 -O2 -mips2 -G 0 -non_shared',exit_code=p.returncode))
(base/'B17proof.json').write_text(json.dumps(rows,indent=2));print(json.dumps(rows,indent=2))
