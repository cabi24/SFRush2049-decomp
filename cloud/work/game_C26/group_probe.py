exec(open(str(__import__('pathlib').Path.home()/'agents/C/scratch/game-C26/probe.py')).read().split('out=[]')[0])
os.environ['IDO_DIR']=str(tk/'ido');obj=p/'vector_group.o';score.compile_group(p/'vector_group',obj);rows=[]
for fn in ['random_int','func_800A61B0']:
 scorer=scoring.Scorer(target_o=str(p/(fn+'.target.o')),stack_differences=True,algorithm='difflib',debug_mode=False,ign_branch_targets=True,objdump_command=scoring.objdump_command()+' --disassemble='+fn)
 r={'function':fn,'strict_score':scorer.score(str(obj))[0],'linked':asdict(score.compare(obj,fn,show=0))};print(json.dumps(r));rows.append(r)
(p/'group_baseline.json').write_text(json.dumps(rows,indent=2))
