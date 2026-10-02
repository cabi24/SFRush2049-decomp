import json,sys,os,hashlib
from pathlib import Path
p=Path.home()/'agents/C/scratch/game-C66';wt=Path.home()/'agents/C/wt';tk=Path.home()/'rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5'
os.environ['CONVEYOR_TOOLKIT']=str(tk);sys.path.insert(0,str(wt/'tools/conveyor/jobs'));import scoring
out={v:scoring.score(p/'func_800BFD8C.target.o',p/(v+'.o'),stack_differences=True) for v in ['real_group','scoped','weights','typed','dot_linear','loop_pointer']}
(p/'strict.json').write_text(json.dumps(out,indent=2)+'\n');print(out)
