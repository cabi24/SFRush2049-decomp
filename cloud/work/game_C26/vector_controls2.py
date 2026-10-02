exec(open(str(__import__('pathlib').Path.home()/'agents/C/scratch/game-C26/probe.py')).read().split('out=[]')[0])
base=(p/'indexed_three.c').read_text()
variants={}
rows=[]
variants['indexed_flat']=base.replace('{ f32 x = direction[0], y = direction[1], z = direction[2];\n    sum = z*z + (x*x + y*y); }','sum = direction[2]*direction[2] + (direction[0]*direction[0] + direction[1]*direction[1]);')
variants['named_components']=base.replace('{ f32 x = direction[0], y = direction[1], z = direction[2];\n    sum = z*z + (x*x + y*y); }','sum = z*z + (x*x + y*y);').replace('f32 scale, sum, factor;','f32 scale, sum, factor;\n    f32 x, y, z;').replace('sum = z*z','x = direction[0]; y = direction[1]; z = direction[2];\n    sum = z*z')
for key,txt in variants.items():
 src=p/(key+'.c');obj=p/(key+'.o');src.write_text(txt);cp=subprocess.run([str(tk/'ido/cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared','-Wab,-r4300_mul',str(src),'-o',str(obj)],capture_output=True,text=True);r={'control':key,'compile_exit':cp.returncode,'stderr':cp.stderr}
 if not cp.returncode:r.update(strict=scoring.score(p/'random_int.target.o',obj,stack_differences=True),linked=asdict(score.compare(obj,'random_int',show=0)))
 print(json.dumps(r));rows.append(r)
(p/'vector_controls2.json').write_text(json.dumps(rows,indent=2))
