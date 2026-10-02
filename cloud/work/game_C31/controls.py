exec(open(str(__import__('pathlib').Path.home()/'agents/C/scratch/game-C31/probe.py')).read().split('out=[]')[0])
base=(p/'func_800B7FF8.bits.c').read_text();variants={}
variants['local_delta']=base.replace('void func_800B7FF8(void) {', 'void func_800B7FF8(void) {\n    f32 *delta = &D_8002EB94;').replace('D_80123DE0 * D_8002EB94','D_80123DE0 * *delta')
variants['local_delta_inner']=base.replace('if (!D_801170FC) D_80116178 += D_80123DE0 * D_8002EB94;', '{ f32 *delta = &D_8002EB94;\n        if (!D_801170FC) D_80116178 += D_80123DE0 * *delta; }')
variants['explicit_delta_cast']=base.replace('D_80123DE0 * D_8002EB94','D_80123DE0 * *(f32 *)&D_8002EB94')
rows=[]
for key,txt in variants.items():
 src=p/(key+'.c');obj=p/(key+'.o');src.write_text(txt);cp=subprocess.run([str(tk/'ido/cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared','-Wab,-r4300_mul',str(src),'-o',str(obj)],capture_output=True,text=True);r={'control':key,'compile_exit':cp.returncode,'stderr':cp.stderr}
 if not cp.returncode:r.update(strict=scoring.score(p/'func_800B7FF8.target.o',obj,stack_differences=True),linked=asdict(score.compare(obj,'func_800B7FF8',show=0)))
 print(json.dumps(r));rows.append(r)
(p/'controls.json').write_text(json.dumps(rows,indent=2))
