exec(open(str(__import__('pathlib').Path.home()/'agents/C/scratch/game-C29/probe.py')).read().split('out=[]')[0])
base=(p/'audio_output_setup.chosen.c').read_text();variants={}
variants['chosen_if']=base.replace('chosen = list ? list : D_801527C8;', 'if (list) chosen = list; else chosen = D_801527C8;')
variants['unsigned_sum']=base.replace('s32 count;', 'unsigned int count;').replace('s32 audio_output_setup', 'unsigned int audio_output_setup')
variants['count_before_head']=base.replace('p = chosen->head;\n    count = 0;', 'count = 0;\n    p = chosen->head;')
variants['while_loop']=base.replace('for (; p; p = p->next) {', 'while (p) {').replace('if (!p->excluded) count += p->count;', 'if (!p->excluded) count += p->count;\n        p = p->next;')
rows=[]
for key,txt in variants.items():
 src=p/(key+'.c');obj=p/(key+'.o');src.write_text(txt);cp=subprocess.run([str(tk/'ido/cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared','-Wab,-r4300_mul',str(src),'-o',str(obj)],capture_output=True,text=True);r={'control':key,'compile_exit':cp.returncode,'stderr':cp.stderr}
 if not cp.returncode:r.update(strict=scoring.score(p/'audio_output_setup.target.o',obj,stack_differences=True),linked=asdict(score.compare(obj,'audio_output_setup',show=0)))
 print(json.dumps(r));rows.append(r)
(p/'controls.json').write_text(json.dumps(rows,indent=2))
