exec(open('/home/cburnes/agents/C/scratch/game-C25/check.py').read().split('results=[]')[0])
from dataclasses import asdict
rr=[]
for n in ['vector_normalize_length','camera_update_d']:
 base=(p/(n+'.c')).read_text();variants={}
 if n=='vector_normalize_length':
  s=base.replace('    f32 temp_f0_2;\n','').replace('temp_f0_2','temp_f0').replace('    f32 temp_f2_2;\n','').replace('temp_f2_2','temp_f2');variants['reuse_actual_scalars']=s
  variants['typed_matrix']=s.replace('void *arg0, void *arg1','f32 *arg0, f32 *arg1').replace('void *temp_a0','f32 *temp_a0').replace('(u8 *) arg1 + 0x18','arg1 + 6')
 else:
  s=base
  for i in range(2,7):s=s.replace('    f32 var_f0_'+str(i)+';\n','').replace('var_f0_'+str(i),'var_f0')
  variants['reuse_clamp']=s
  for v in ['temp_f20','temp_f22','temp_f24']:s=s.replace('    f32 '+v+'_2;\n','').replace(v+'_2',v)
  variants['reuse_normalized_components']=s
 for label,s in variants.items():
  src=p/(n+'.'+label+'.c');src.write_text(s);obj=src.with_suffix('.o');cp=subprocess.run([str(tk/'ido/cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared','-Wab,-r4300_mul','-I'+str(p/'include'),str(src),'-o',str(obj)],capture_output=True,text=True);assert cp.returncode==0,cp.stderr
  r={'function':n,'variant':label,'strict_score':scoring.score(p/(n+'.target.o'),obj,stack_differences=True),'linked':asdict(cloudscore.compare(obj,n,show=0))};print(json.dumps(r),flush=True);rr.append(r)
(p/'directed_controls.json').write_text(json.dumps(rr,indent=2)+'\n')
