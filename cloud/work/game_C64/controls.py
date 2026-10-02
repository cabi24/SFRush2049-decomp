import os,sys,json,subprocess,hashlib
from pathlib import Path
from dataclasses import asdict
p=Path.home()/'agents/C/scratch/game-C64';wt=Path.home()/'agents/C/wt';tk=Path.home()/'rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5'
os.environ['CONVEYOR_TOOLKIT']=str(tk);sys.path[:0]=[str(wt/'tools/conveyor/jobs'),str(wt/'tools/cloud')]
import scoring
import score
out=[]
for n in ['func_800D169C.scalar_copy', 'func_800D169C.early_copy', 'func_800D169C.pointers', 'func_800D169C.halves', 'func_800D169C.O3', 'func_800D169C.arrays', 'func_800D169C.dot_order', 'func_800D169C.loop_scoped']:
 src=p/(n+'.c');obj=p/(n+'.o');flags=src.read_text().splitlines()[0][10:-3]
 cmd=[str(tk/'ido/cc'),'-c',*flags.split(),str(src),'-o',str(obj)];cp=subprocess.run(cmd,capture_output=True,text=True)
 r={'function':n,'flags':flags,'compile_exit':cp.returncode,'stderr':cp.stderr}
 if not cp.returncode:r.update(strict=scoring.score(p/'func_800D169C.target.o',obj,stack_differences=True),linked=asdict(score.compare(obj,'func_800D169C',show=0)),source_sha256=hashlib.sha256(src.read_bytes()).hexdigest(),target_sha256=hashlib.sha256((p/'func_800D169C.target.o').read_bytes()).hexdigest())
 out.append(r);print(json.dumps(r))
(p/'controls.json').write_text(json.dumps(out,indent=2))
