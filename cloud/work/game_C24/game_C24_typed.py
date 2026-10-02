exec(open('/home/cburnes/agents/C/scratch/game-C24/check.py').read().split('results=[]')[0])
from dataclasses import asdict
os.environ['IDO_DIR']=str(tk/'ido');base=(p/'group_canonical_order/group.c').read_text()
typed=base.replace('extern s32 D_8012E638','extern f32 D_8012E638[][3]').replace('extern s8 D_8012E670','extern s8 D_8012E670[]').replace('extern f32 D_80152720','extern f32 D_80152720[]').replace('(&D_8012E670)','D_8012E670').replace('(&D_80152720)','D_80152720').replace('(s32 *) ((temp_v1 * 0xC) + (u8 *) &D_8012E638)','D_8012E638[temp_v1]')
for var in ['temp_v0','temp_v0_2','temp_v0_3']: typed=typed.replace('void *'+var+';','f32 *'+var+';')
variants={'typed_global_arrays':typed,'one_vector_pointer':typed.replace('temp_v0_2','temp_v0').replace('temp_v0_3','temp_v0').replace('    f32 *temp_v0;\n    f32 *temp_v0;\n    f32 *temp_v0;','    f32 *temp_v0;')}
variants['typed_argument']=typed.replace('void *arg2,','f32 *arg2,')
rr=[]
for label,s in variants.items():
 d=p/('group_'+label);d.mkdir(exist_ok=True);(d/'group.c').write_text(s);(d/'group.json').write_bytes((p/'vector_group/group.json').read_bytes());obj=d/'candidate.o';cloudscore.compile_group(d,obj)
 n='func_800E9C70';scorer=scoring.Scorer(target_o=str(p/(n+'.target.o')),stack_differences=True,algorithm='difflib',debug_mode=False,ign_branch_targets=True,objdump_command=scoring.objdump_command()+' --disassemble='+n)
 r={'variant':label,'function':n,'strict_score':scorer.score(str(obj))[0],'linked':asdict(cloudscore.compare(obj,n,show=0))};print(json.dumps(r),flush=True);rr.append(r)
(p/'typed_controls.json').write_text(json.dumps(rr,indent=2)+'\n')
