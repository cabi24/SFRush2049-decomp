exec(open(str(__import__('pathlib').Path.home()/'agents/C/scratch/game-C29/probe.py')).read().split('out=[]')[0])
base=(p/'audio_output_setup.chosen.c').read_text();variants={}
variants['register_list']=base.replace('AudioList *list)', 'register AudioList *list)')
variants['register_locals']=base.replace('AudioEntry *p;', 'register AudioEntry *p;').replace('AudioList *chosen;\n    s32 count;', 'AudioList *chosen;\n    register s32 count;').replace('AudioList *chosen;', 'register AudioList *chosen;')

rows=[]
for key,txt in variants.items():
 src=p/(key+'.c');obj=p/(key+'.o');src.write_text(txt);cp=subprocess.run([str(tk/'ido/cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared','-Wab,-r4300_mul',str(src),'-o',str(obj)],capture_output=True,text=True);r={'control':key,'compile_exit':cp.returncode,'stderr':cp.stderr}
 if not cp.returncode:r.update(strict=scoring.score(p/'audio_output_setup.target.o',obj,stack_differences=True),linked=asdict(score.compare(obj,'audio_output_setup',show=0)))
 print(json.dumps(r));rows.append(r)
(p/'register_controls.json').write_text(json.dumps(rows,indent=2))
