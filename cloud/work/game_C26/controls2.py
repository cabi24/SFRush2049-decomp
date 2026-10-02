exec(open(str(__import__('pathlib').Path.home()/'agents/C/scratch/game-C26/probe.py')).read().split('out=[]')[0])
base=(p/'func_800B23E0.c').read_text();rows=[]
base=(p/'float_local.c').read_text()
variants={}
variants['pointer_mask']=base.replace('u32 mask = D_80123418[index];','u32 *mask = &D_80123418[index];').replace('mask &','*mask &')
variants['explicit_loop']=base.replace('do {','for (;;) {').replace('} while (!(mask & (1 << choice)));','if (mask & (1 << choice)) break;\n    }')
variants['signed_choice']=base.replace('u8 choice;','s32 choice;').replace('choice = (u32)value;','choice = (u8)(u32)value;')
for key,txt in variants.items():
 src=p/(key+'.c');obj=p/(key+'.o');src.write_text(txt);cp=subprocess.run([str(tk/'ido/cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared','-Wab,-r4300_mul',str(src),'-o',str(obj)],capture_output=True,text=True);r={'control':key,'compile_exit':cp.returncode,'stderr':cp.stderr}
 if not cp.returncode:r.update(strict=scoring.score(p/'func_800B23E0.target.o',obj,stack_differences=True),linked=asdict(score.compare(obj,'func_800B23E0',show=0)))
 print(json.dumps(r));rows.append(r)
(p/'controls2.json').write_text(json.dumps(rows,indent=2))
