exec(open(str(__import__('pathlib').Path.home()/'agents/C/scratch/game-C28/probe.py')).read().split('out=[]')[0])
base=(p/'cursor_loop.c').read_text();variants={}
variants['conditional_cursor']=base.replace('f32 (*cursor)[2] = table;', 'f32 (*cursor)[2];').replace('while ((*cursor)[0] < value && (*cursor)[0] < 32000.0f) { index++; cursor++; }', 'if (table[index][0] < value && table[index][0] < 32000.0f) { cursor = table + index; do { index++; cursor++; } while ((*cursor)[0] < value && (*cursor)[0] < 32000.0f); }')

rows=[]
for key,txt in variants.items():
 src=p/(key+'.c');obj=p/(key+'.o');src.write_text(txt);cp=subprocess.run([str(tk/'ido/cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared','-Wab,-r4300_mul',str(src),'-o',str(obj)],capture_output=True,text=True);r={'control':key,'compile_exit':cp.returncode,'stderr':cp.stderr}
 if not cp.returncode:r.update(strict=scoring.score(p/'func_800F6928.target.o',obj,stack_differences=True),linked=asdict(score.compare(obj,'func_800F6928',show=0)))
 print(json.dumps(r));rows.append(r)
(p/'controls2.json').write_text(json.dumps(rows,indent=2))
