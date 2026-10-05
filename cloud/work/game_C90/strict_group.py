import json,os,sys
from pathlib import Path
p=Path.home()/'agents/C/scratch/game-C90'
tk=Path.home()/'rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5'
os.environ['CONVEYOR_TOOLKIT']=str(tk)
sys.path.insert(0,str(Path.home()/'agents/C/wt/tools/conveyor/jobs'))
import scoring
s=scoring.score(p/'func_800A1E94.target.o',p/'group/group.o',stack_differences=True)
r={'actual_production_group_strict_stack_differences':s,'score_exit':0}
(p/'strict_group.json').write_text(json.dumps(r,indent=2)+'\n')
print(json.dumps(r))
