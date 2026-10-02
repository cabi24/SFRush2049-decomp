exec(open('/home/cburnes/agents/C/scratch/game-C25/check.py').read().split('results=[]')[0])
from dataclasses import asdict
os.environ['IDO_DIR']=str(tk/'ido');base=(p/'velocity_group/group.c').read_text();s=base.replace('    f32 sp24;\n    f32 sp20;\n    f32 sp1C;','    f32 delta[3];').replace('sp1C','delta[0]').replace('sp20','delta[1]').replace('sp24','delta[2]')
d=p/'velocity_vector';d.mkdir(exist_ok=True);(d/'group.c').write_text(s);(d/'group.json').write_bytes((p/'velocity_group/group.json').read_bytes());obj=d/'candidate.o';cloudscore.compile_group(d,obj);n='func_800E114C';scorer=scoring.Scorer(target_o=str(p/(n+'.target.o')),stack_differences=True,algorithm='difflib',debug_mode=False,ign_branch_targets=True,objdump_command=scoring.objdump_command()+' --disassemble='+n)
r={'variant':'real_delta_vector','strict_score':scorer.score(str(obj))[0],'linked':asdict(cloudscore.compare(obj,n,show=4))};print(json.dumps(r));(p/'delta_vector.json').write_text(json.dumps(r,indent=2)+'\n')
