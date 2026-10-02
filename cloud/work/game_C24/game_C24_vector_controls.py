exec(open('/home/cburnes/agents/C/scratch/game-C24/check.py').read().split('results=[]')[0])
from dataclasses import asdict
base=(p/'func_8008C544.c').read_text();variants={'canonical_sum':base.replace('m[7]*v[2] + (-v[0]*m[1] + v[1]*m[4])','(-v[0]*m[1] + v[1]*m[4]) + m[7]*v[2]').replace('m[8]*v[2] + (-v[0]*m[2] + v[1]*m[5])','(-v[0]*m[2] + v[1]*m[5]) + m[8]*v[2]')}
variants['canonical_mul']=variants['canonical_sum'].replace('m[6]*v[2]','v[2]*m[6]').replace('m[7]*v[2]','v[2]*m[7]').replace('m[8]*v[2]','v[2]*m[8]')
variants['explicit_partial']=variants['canonical_sum'].replace('void func_8008C544(f32 *v, f32 *out, f32 *m) {','void func_8008C544(f32 *v, f32 *out, f32 *m) {\n f32 partial;').replace('out[1] = (-v[0]*m[1] + v[1]*m[4]) + m[7]*v[2];','partial=-v[0]*m[1] + v[1]*m[4];\n out[1] = partial + m[7]*v[2];').replace('out[2] = (-v[0]*m[2] + v[1]*m[5]) + m[8]*v[2];','partial=-v[0]*m[2] + v[1]*m[5];\n out[2] = partial + m[8]*v[2];')
rr=[]
for label,s in variants.items():
 src=p/('func_8008C544.'+label+'.c');src.write_text(s);obj=src.with_suffix('.o');cp=subprocess.run([str(tk/'ido/cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared','-Wab,-r4300_mul',str(src),'-o',str(obj)],capture_output=True,text=True);assert cp.returncode==0,cp.stderr
 r={'variant':label,'strict_score':scoring.score(p/'func_8008C544.target.o',obj,stack_differences=True),'linked':asdict(cloudscore.compare(obj,'func_8008C544',show=0))};print(json.dumps(r),flush=True);rr.append(r)
(p/'vector_controls.json').write_text(json.dumps(rr,indent=2)+'\n')
