exec(open('/home/cburnes/agents/C/scratch/game-C24/check.py').read().split('results=[]')[0])
from dataclasses import asdict
os.environ['IDO_DIR']=str(tk/'ido');base=(p/'vector_group/group.c').read_text();base=base.replace('sqrtf(v[2]*v[2]+(v[0]*v[0]+v[1]*v[1]))','sqrtf(v[0]*v[0]+v[1]*v[1]+v[2]*v[2])')
struct=base.replace('extern f32 func_8008B3C8(f32 *);','typedef struct C24Vector { f32 x,y,z;} C24Vector;\nextern f32 func_8008B3C8(C24Vector *);').replace('extern void func_800E8D50(void*,void*,s32,f32*);','extern void func_800E8D50(void*,void*,s32,C24Vector*);').replace('f32 vector[3];','C24Vector vector;').replace('func_8008B3C8(vector)','func_8008B3C8(&vector)').replace('arg3, vector','arg3, &vector').replace('f32 func_8008B3C8(f32 *v)','f32 func_8008B3C8(C24Vector *v)')
for i,f in enumerate(['x','y','z']): struct=struct.replace('vector['+str(i)+']','vector.'+f).replace('v['+str(i)+']','v->'+f)
rr=[]
for label,s in [('canonical_order',base),('struct_vector',struct)]:
 d=p/('group_'+label);d.mkdir(exist_ok=True);(d/'group.c').write_text(s);(d/'group.json').write_bytes((p/'vector_group/group.json').read_bytes());obj=d/'candidate.o';cloudscore.compile_group(d,obj)
 row=[]
 for n in ['func_800E9C70','func_8008B3C8']:
  scorer=scoring.Scorer(target_o=str(p/(n+'.target.o')),stack_differences=True,algorithm='difflib',debug_mode=False,ign_branch_targets=True,objdump_command=scoring.objdump_command()+' --disassemble='+n)
  r={'variant':label,'function':n,'strict_score':scorer.score(str(obj))[0],'linked':asdict(cloudscore.compare(obj,n,show=0))};print(json.dumps(r),flush=True);rr.append(r)
(p/'group_controls.json').write_text(json.dumps(rr,indent=2)+'\n')
