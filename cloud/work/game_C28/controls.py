exec(open(str(__import__('pathlib').Path.home()/'agents/C/scratch/game-C28/probe.py')).read().split('out=[]')[0])
base=(p/'func_800F6928.c').read_text();variants={}
variants['for_loop']=base.replace('while (table[index][0] < value && table[index][0] < 32000.0f) index++;','for (index = 0; table[index][0] < value && table[index][0] < 32000.0f; index++);')
variants['named_interpolation']=base.replace('f32 (*table)[2] = D_80114750;','f32 (*table)[2] = D_80114750;\n    f32 prevx, prevy;').replace('value = (value - table[index-1][0]) * (table[index][1] - table[index-1][1]) / (table[index][0] - table[index-1][0]) + table[index-1][1];','prevx = table[index-1][0];\n        prevy = table[index-1][1];\n        value = (value - prevx) * (table[index][1] - prevy) / (table[index][0] - prevx) + prevy;')
variants['cursor_loop']=variants['named_interpolation'].replace('f32 prevx, prevy;','f32 prevx, prevy;\n    f32 (*cursor)[2] = table;').replace('while (table[index][0] < value && table[index][0] < 32000.0f) index++;','while ((*cursor)[0] < value && (*cursor)[0] < 32000.0f) { index++; cursor++; }')
variants['initial_loop']=variants['named_interpolation'].replace('while (table[index][0] < value && table[index][0] < 32000.0f) index++;','if (table[0][0] < value && table[0][0] < 32000.0f) {\n        do { index++; } while (table[index][0] < value && table[index][0] < 32000.0f);\n    }')
rows=[]
for key,txt in variants.items():
 src=p/(key+'.c');obj=p/(key+'.o');src.write_text(txt);cp=subprocess.run([str(tk/'ido/cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared','-Wab,-r4300_mul',str(src),'-o',str(obj)],capture_output=True,text=True);r={'control':key,'compile_exit':cp.returncode,'stderr':cp.stderr}
 if not cp.returncode:r.update(strict=scoring.score(p/'func_800F6928.target.o',obj,stack_differences=True),linked=asdict(score.compare(obj,'func_800F6928',show=0)))
 print(json.dumps(r));rows.append(r)
(p/'controls.json').write_text(json.dumps(rows,indent=2))
