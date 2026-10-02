exec(open(str(__import__('pathlib').Path.home()/'agents/C/scratch/game-C30/probe.py')).read().split('out=[]')[0])
base=(p/'func_8009E8B4.c').read_text();variants={}
variants['sdk_masks']=base.replace('((u32)second >> 16)', '((second >> 16) & 0xffff)').replace('(first << 16) |', '((first << 16) & 0xffff0000) |')
variants['fixed_offsets']=base.replace('u32 *fraction = fixed + 8;','').replace('*fraction++ =','integer[7] =').replace('*fraction =','integer[8] =')
variants['signed_pointer']=variants['sdk_masks'].replace('u32 *integer = fixed;', 's32 *integer = (s32 *)fixed;').replace('u32 *fraction = fixed + 8;', 's32 *fraction = (s32 *)(fixed + 8);')
variants['flat_input']=base.replace('f32 matrix[4][4]', 'f32 *matrix').replace('matrix[i][0]', 'matrix[0]').replace('matrix[i][1]', 'matrix[1]').replace('matrix[i][2]', 'matrix[2]').replace('*fraction++ = first << 16;', '*fraction++ = first << 16;\n        matrix += 4;')
rows=[]
for key,txt in variants.items():
 src=p/(key+'.c');obj=p/(key+'.o');src.write_text(txt);cp=subprocess.run([str(tk/'ido/cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared','-Wab,-r4300_mul',str(src),'-o',str(obj)],capture_output=True,text=True);r={'control':key,'compile_exit':cp.returncode,'stderr':cp.stderr}
 if not cp.returncode:r.update(strict=scoring.score(p/'func_8009E8B4.target.o',obj,stack_differences=True),linked=asdict(score.compare(obj,'func_8009E8B4',show=0)))
 print(json.dumps(r));rows.append(r)
(p/'controls.json').write_text(json.dumps(rows,indent=2))
