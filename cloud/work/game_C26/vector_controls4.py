exec(open(str(__import__('pathlib').Path.home()/'agents/C/scratch/game-C26/probe.py')).read().split('out=[]')[0])
base=(p/'reuse_sum.c').read_text()
variants={}
rows=[]
variants['indexed_pointers']=base.replace('int i;','int i;\n    f32 *dst, *src;').replace('for (i = 0; i < 3; i++) vector[i] = direction[i] * factor;','dst = vector; src = direction;\n        for (i = 0; i < 3; i++) *dst++ = *src++ * factor;')
variants['explicit_xyz']=base.replace('factor = direction[2]*direction[2] + (direction[0]*direction[0] + direction[1]*direction[1]);','factor = direction[0]*direction[0] + direction[1]*direction[1];\n    factor = direction[2]*direction[2] + factor;')
for key,txt in variants.items():
 src=p/(key+'.c');obj=p/(key+'.o');src.write_text(txt);cp=subprocess.run([str(tk/'ido/cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared','-Wab,-r4300_mul',str(src),'-o',str(obj)],capture_output=True,text=True);r={'control':key,'compile_exit':cp.returncode,'stderr':cp.stderr}
 if not cp.returncode:r.update(strict=scoring.score(p/'random_int.target.o',obj,stack_differences=True),linked=asdict(score.compare(obj,'random_int',show=0)))
 print(json.dumps(r));rows.append(r)
(p/'vector_controls4.json').write_text(json.dumps(rows,indent=2))
