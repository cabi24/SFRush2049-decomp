exec(open('/home/cburnes/agents/C/scratch/game-C24/check.py').read().split('results=[]')[0])
from dataclasses import asdict
os.environ['IDO_DIR']=str(tk/'ido');base=(p/'group_one_vector_pointer/group.c').read_text()
old='''    vector[0] = M2C_FIELD(temp_v0, f32 *, 0) - M2C_FIELD(arg2, f32 *, 0);
    vector[1] = M2C_FIELD(temp_v0, f32 *, 4) - M2C_FIELD(arg2, f32 *, 4);
    D_80152720[temp_v1] = 0.0f;
    vector[2] = M2C_FIELD(temp_v0, f32 *, 8) - M2C_FIELD(arg2, f32 *, 8);
    if (D_8012E670[temp_v1] != 0) {'''
variants={}
for mode in ['flag_before','third_before_zero','reuse_third','flag_after_first']:
 s=base
 if mode.startswith('flag_'):
  s=s.replace('s8 temp_v1;','s8 temp_v1;\n    s8 enabled;')
  block=old.replace('if (D_8012E670[temp_v1] != 0)', 'if(enabled != 0)')
  if mode=='flag_before':block='    enabled=D_8012E670[temp_v1];\n'+block
  else:block=block.replace('    vector[1] =','    enabled=D_8012E670[temp_v1];\n    vector[1] =')
  s=s.replace(old,block)
 elif mode=='third_before_zero':s=s.replace(old,old.replace('    D_80152720[temp_v1] = 0.0f;\n','').replace('    if (','    D_80152720[temp_v1] = 0.0f;\n    if ('))
 else:s=s.replace(old,old.replace('    vector[0] =','    temp_f2=M2C_FIELD(temp_v0, f32 *, 8);\n    vector[0] =').replace('vector[2] = M2C_FIELD(temp_v0, f32 *, 8)','vector[2] = temp_f2'))
 variants[mode]=s
rr=[]
for label,s in variants.items():
 d=p/('group_'+label);d.mkdir(exist_ok=True);(d/'group.c').write_text(s);(d/'group.json').write_bytes((p/'vector_group/group.json').read_bytes());obj=d/'candidate.o';cloudscore.compile_group(d,obj)
 n='func_800E9C70';scorer=scoring.Scorer(target_o=str(p/(n+'.target.o')),stack_differences=True,algorithm='difflib',debug_mode=False,ign_branch_targets=True,objdump_command=scoring.objdump_command()+' --disassemble='+n)
 r={'variant':label,'function':n,'strict_score':scorer.score(str(obj))[0],'linked':asdict(cloudscore.compare(obj,n,show=0))};print(json.dumps(r),flush=True);rr.append(r)
(p/'schedule_controls.json').write_text(json.dumps(rr,indent=2)+'\n')
