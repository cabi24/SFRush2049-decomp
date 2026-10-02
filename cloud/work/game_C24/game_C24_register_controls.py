exec(open('/home/cburnes/agents/C/scratch/game-C24/check.py').read().split('results=[]')[0])
from dataclasses import asdict
n='func_800ACFF8';base=(p/(n+'.c')).read_text();variants={'register_temporaries':base.replace('f32 temp_','register f32 temp_'),'register_arguments':base.replace('(f32 arg0, f32 arg1,','(register f32 arg0, register f32 arg1,'),'register_all':base.replace('    f32 ','    register f32 ')};rr=[]
for label,s in variants.items():
 src=p/(n+'.'+label+'.c');src.write_text(s);obj=src.with_suffix('.o');cp=subprocess.run([str(tk/'ido/cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared','-Wab,-r4300_mul',str(src),'-o',str(obj)],capture_output=True,text=True);assert cp.returncode==0,cp.stderr
 r={'function':n,'variant':label,'strict_score':scoring.score(p/(n+'.target.o'),obj,stack_differences=True),'linked':asdict(cloudscore.compare(obj,n,show=0))};print(json.dumps(r),flush=True);rr.append(r)
(p/'register_controls.json').write_text(json.dumps(rr,indent=2)+'\n')
