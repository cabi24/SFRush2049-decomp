import os,sys,json,subprocess,hashlib
from pathlib import Path
from dataclasses import asdict
p=Path.home()/'agents/C/scratch/static-C94';wt=Path.home()/'agents/C/wt';tk=Path.home()/'rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5'
os.environ['CONVEYOR_TOOLKIT']=str(tk);sys.path[:0]=[str(wt/'tools/conveyor/jobs'),str(wt/'tools/cloud')]
import scoring
import score
out=[]
for n,variant in [('inflate_free_window','inflate_free_window.'+v) for v in ('debug_o2','volatile_debug_o2')]:
 src=p/(variant+'.c');obj=p/(variant+'.o');flags=src.read_text().splitlines()[0][10:-3]
 cmd=[str(tk/'ido/cc'),'-c',*flags.split(),'-I'+str(p/'include'),'-I'+str(p),str(src),'-o',str(obj)];cp=subprocess.run(cmd,capture_output=True,text=True)
 r={'function':n,'variant':variant,'flags':flags,'compile_exit':cp.returncode,'stderr':cp.stderr}
 if not cp.returncode:r.update(strict=scoring.score(p/(n+'.target.o'),obj,stack_differences=True),raw_word_diff=sum(a!=b for a,b in zip(score.text_words(p/(n+'.target.o')),score.text_words(obj)))+abs(len(score.text_words(p/(n+'.target.o')))-len(score.text_words(obj))),candidate_words=len(score.text_words(obj)),source_sha256=hashlib.sha256(src.read_bytes()).hexdigest(),target_sha256=hashlib.sha256((p/(n+'.target.o')).read_bytes()).hexdigest())
 out.append(r);print(json.dumps(r))
(p/'debug_o2.json').write_text(json.dumps(out,indent=2))
