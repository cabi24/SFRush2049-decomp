from pathlib import Path
import subprocess,re,json
root=Path.home()/'agents/B/scratch/codex_B'; root.mkdir(exist_ok=True)
base=Path.home()/'agents/wb/base'; wt=Path.home()/'agents/B/wt'
flags='-g0 -O2 -mips2 -G 0 -non_shared'
records=[]
def run(fn,label,s):
 p=root/(fn+'_'+label+'.c'); p.write_text(s)
 r=subprocess.run(['python3','tools/cloud/score.py','fn',str(p),fn,'--flags',flags],cwd=wt,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
 out=r.stdout.strip().splitlines()[-1] if r.stdout.strip() else 'EMPTY'
 records.append(dict(fn=fn,label=label,result=out,path=str(p))); print(fn,label,out,flush=True)
 (root/'scores.json').write_text(json.dumps(records,indent=2))
fn='func_800D18D8'; s=(base/(fn+'.c')).read_text().replace('    s32 *sp1C;\n','').replace('    sp1C = temp_a1;\n','')
run(fn,'drop',s)
for sym in ['D_801460F8','D_80146104','D_80110258']:
 for typ in ['s32','u32','volatile s32']:
  v=s.replace('extern s32 '+sym+';',typ+' '+sym+';')
  run(fn,'def_'+sym+'_'+typ.replace(' ','_'),v)
for typ in ['s32','u32','volatile s32']:
 v=s
 for sym in ['D_801460F8','D_80146104','D_80110258']: v=v.replace('extern s32 '+sym+';',typ+' '+sym+';')
 run(fn,'all_def_'+typ.replace(' ','_'),v)
for typ in ['u8','u16','s32','u32','s64','f32']:
 v=s.replace('    s32 *temp_a1;','    '+typ+' pad;\n    s32 *temp_a1;')
 run(fn,'pad_'+typ,v)
fn='state_update_global'; s=(base/(fn+'.c')).read_text()
best=s.replace('    v = arg0->f1A;\n    t = D_80149D98 != 0;\n    if (t != v) {','    if ((v = arg0->f1A) != (t = D_80149D98 != 0)) {')
run(fn,'assignment',best)
for i,cond in enumerate(['(t = D_80149D98 != 0) != (v = arg0->f1A)','(v = arg0->f1A) - (t = D_80149D98 != 0)','(v = arg0->f1A) ^ (t = D_80149D98 != 0)','(t = !!D_80149D98) != (v = arg0->f1A)','(v = arg0->f1A) != (t = (u32)D_80149D98 > 0)','(v = arg0->f1A) != (t = D_80149D98 ? 1 : 0)']):
 run(fn,'cond_'+str(i),best.replace('(v = arg0->f1A) != (t = D_80149D98 != 0)',cond))
for i,expr in enumerate(['if (v & 255) {}','if (v + 1) {}','if (v == 0) {}','if (t + 1) {}','if (t == 0) {}','if (arg0) {}']):
 run(fn,'cost_'+str(i),best.replace('    if ((v =', '    '+expr+'\n    if ((v ='))
for typ in ['u8','s8','u32','s32']:
 for lhs in ['t','v']:
  run(fn,'type_'+lhs+'_'+typ,best.replace('s32 '+lhs+';',typ+' '+lhs+';'))
for decl in ['s32 D_801497F4; s32 D_80149D98;','extern s32 D_801497F4; volatile s32 D_80149D98;','extern s32 D_801497F4; extern volatile s32 D_80149D98;']:
 run(fn,'global_'+str(len(records)),best.replace('extern s32 D_801497F4, D_80149D98;',decl))
