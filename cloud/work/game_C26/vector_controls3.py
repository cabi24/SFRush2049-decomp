exec(open(str(__import__('pathlib').Path.home()/'agents/C/scratch/game-C26/probe.py')).read().split('out=[]')[0])
base=(p/'indexed_flat.c').read_text()
variants={}
rows=[]
variants['reuse_sum']=base.replace('f32 scale, sum, factor;','f32 scale, factor;').replace('sum','factor')
variants['reuse_scale']=base.replace('f32 scale, sum, factor;','f32 scale, sum;').replace('factor = scale / sqrtf(sum);','scale = scale / sqrtf(sum);').replace('* factor;','* scale;')
for key,txt in variants.items():
 src=p/(key+'.c');obj=p/(key+'.o');src.write_text(txt);cp=subprocess.run([str(tk/'ido/cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared','-Wab,-r4300_mul',str(src),'-o',str(obj)],capture_output=True,text=True);r={'control':key,'compile_exit':cp.returncode,'stderr':cp.stderr}
 if not cp.returncode:r.update(strict=scoring.score(p/'random_int.target.o',obj,stack_differences=True),linked=asdict(score.compare(obj,'random_int',show=0)))
 print(json.dumps(r));rows.append(r)
(p/'vector_controls3.json').write_text(json.dumps(rows,indent=2))
