exec(open(str(__import__('pathlib').Path.home()/'agents/C/scratch/game-C26/probe.py')).read().split('out=[]')[0])
base=(p/'func_800B23E0.c').read_text();rows=[]
variants={'signed_mask':base.replace('extern u32 D_80123418[]','extern s32 D_80123418[]').replace('u32 mask =','s32 mask ='),'wide_return':base.replace('u8 func_800B23E0','u32 func_800B23E0'),'float_local':base.replace('u8 choice;','u8 choice;\n    f32 value;').replace('choice = (u32)((f32)((D_8011735C >> 16) & 0x7fff) * 32.0f / 32768.0f);','value = (f32)((D_8011735C >> 16) & 0x7fff) * 32.0f / 32768.0f;\n        choice = (u32)value;'),'local_seed':base.replace('u8 choice;','u8 choice;\n    s32 seed = D_8011735C;').replace('D_8011735C = D_8011735C * 1103515245U + 12345;','seed = seed * 1103515245U + 12345;\n        D_8011735C = seed;').replace('(D_8011735C >> 16)','(seed >> 16)'),'operand_order':base.replace('mask & (1 << choice)','(1 << choice) & mask')}
for key,txt in variants.items():
 src=p/(key+'.c');obj=p/(key+'.o');src.write_text(txt);cp=subprocess.run([str(tk/'ido/cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared','-Wab,-r4300_mul',str(src),'-o',str(obj)],capture_output=True,text=True);r={'control':key,'compile_exit':cp.returncode,'stderr':cp.stderr}
 if not cp.returncode:r.update(strict=scoring.score(p/'func_800B23E0.target.o',obj,stack_differences=True),linked=asdict(score.compare(obj,'func_800B23E0',show=0)))
 print(json.dumps(r));rows.append(r)
(p/'controls.json').write_text(json.dumps(rows,indent=2))
