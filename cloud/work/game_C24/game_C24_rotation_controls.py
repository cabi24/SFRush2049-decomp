exec(open('/home/cburnes/agents/C/scratch/game-C24/check.py').read().split('results=[]')[0])
from dataclasses import asdict
rr=[]
for n in ['func_800ACFF8','func_800AD090','func_800C40E8']:
 base=(p/(n+'.c')).read_text();variants={}
 for label,vars in [('reuse_first',['temp_f2']),('reuse_pair',['temp_f2','temp_f16'])]:
  s=base
  for v in vars:s=s.replace('    f32 '+v+'_2;\n','').replace(v+'_2',v)
  variants[label]=s
 for label,s in variants.items():
  src=p/(n+'.'+label+'.c');src.write_text(s);obj=src.with_suffix('.o');cp=subprocess.run([str(tk/'ido/cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared','-Wab,-r4300_mul',str(src),'-o',str(obj)],capture_output=True,text=True);assert cp.returncode==0,cp.stderr
  r={'function':n,'variant':label,'strict_score':scoring.score(p/(n+'.target.o'),obj,stack_differences=True),'linked':asdict(cloudscore.compare(obj,n,show=0))};print(json.dumps(r),flush=True);rr.append(r)
(p/'rotation_controls.json').write_text(json.dumps(rr,indent=2)+'\n')
