exec(open('/home/cburnes/agents/B/scratch/codex_B/probe.py').read().split("fn='func_800D18D8'")[0])
fn='func_800CCE5C'; s=(base/(fn+'.c')).read_text(); s=s.replace('    s32 pad;\n','').replace('    s32 pad2;\n','').replace('    u8 *sp1C;\n','').replace('        sp1C = (u8 *) sp24 + 0x3C;\n',''); s=re.sub(r'\bsp1C\b','((u8 *) sp24 + 0x3C)',s)
run(fn,'inline_home',s)
for i,exp in enumerate(['(M2C_FIELD(*arg0, void ***, 8))','(M2C_FIELD(*arg0, void ***, 8) + 0)','(void **)((u32)M2C_FIELD(*arg0, void ***, 8) & 0xFFFFFFFF)','*(void ***)((u8 *)*arg0 + 8)','((void ***)*arg0)[2]']):
 run(fn,'field_'+str(i),s.replace('M2C_FIELD(*arg0, void ***, 8)',exp))
fn='func_8008C680'; s=(base/(fn+'.c')).read_text()
for i,typ in enumerate(['f32','f64','volatile f32']):
 for sym in ['D_801238E4','D_801238E8','D_801238F0']:
  run(fn,'def_'+sym+'_'+str(i),s.replace('extern f32 '+sym+';',typ+' '+sym+';'))
fn='func_8008A704'; s=(base/(fn+'.c')).read_text()
for i,decl in enumerate(['s8 D_8011194C;','volatile s8 D_8011194C;','extern volatile s8 D_8011194C;','s8 D_8011194C = 0;']):
 run(fn,'global_'+str(i),s.replace('extern s8 D_8011194C;',decl))
