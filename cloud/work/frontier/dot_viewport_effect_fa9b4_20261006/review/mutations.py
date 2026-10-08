#!/usr/bin/env python3
import hashlib,json,os,subprocess
from pathlib import Path
import review as r
source=(r.HERE/'frozen_candidate.c').read_text()
MUTATIONS=[
 ('signed-transition',[('s8 transition;','u8 transition;')]),
 ('effect-zero',[('player->effect_index >= 0','player->effect_index > 0')]),
 ('cached-sound-slot',[('D_80152698[model->slot]','D_80152698[slot]'),('entity_flags_apply(45, model->slot, 1, 1)','entity_flags_apply(45, slot, 1, 1)'),('0.0f, 45, model->slot, 0, 128)','0.0f, 45, slot, 0, 128)')]),
 ('lost-captured-slot',[('func_800D5524(model);','func_800D5524(model); slot = model->slot;')]),
 ('cached-loop-count',[('int i;','int i; int count;'),('for (i = 0; i < D_80152744;','count = D_80152744;\n    for (i = 0; i < count;')]),
 ('threshold-inclusive',[('slot < D_8014A108','slot <= D_8014A108')]),
 ('saturating-counter',[('D_8014A118[slot].transitions++;','if (D_8014A118[slot].transitions != 65535) D_8014A118[slot].transitions++;')]),
 ('third-coordinate-omitted',[('effect->saved_position[2] = effect->position[2];',';')]),
 ('wrong-effect-restore',[('player->effect_mode = player->saved_effect_mode;','player->effect_mode = 4;')]),
 ('positive-only-sound',[('if (D_8010FFC0)','if (D_8010FFC0 > 0)')]),
 ('object-sign-reversed',[('header->flags < 0','header->flags >= 0')]),
 ('wrong-flag-mask',[('&= ~32u','&= ~16u')]),
 ('missing-slot-gate',[('(D_8014A110 != 2 || slot == 0)','(D_8014A110 != 2 || 1)')]),
 ('effect-call-order',[('func_8038FCE0();\n        func_80390F60();','func_80390F60();\n        func_8038FCE0();')]),
 ('wrong-scale',[('save_write_data(&slot, 1, 1.0f, 0);','save_write_data(&slot, 1, 0.0f, 0);')]),
 ('model-stride',[('0x808 - 0x7CD','0x80C - 0x7CD')])]
results=[]
for index,(name,changes) in enumerate(MUTATIONS):
 mutant=source
 for old,new in changes:
  assert old in mutant,(name,old);mutant=mutant.replace(old,new)
 src=r.WORK/('mutant-'+str(index)+'.c');src.write_text(mutant)
 lib=r.WORK/('mutant-'+str(index)+'.so')
 subprocess.run(['gcc','-std=c99','-O2','-fPIC','-shared',f'-DCANDIDATE_FILE="{src}"',str(r.HERE/'host_review.c'),'-o',str(lib)],check=True,env=dict(os.environ,TMPDIR=str(r.WORK)))
 try:host=r.Host(lib)
 except AssertionError:
  results.append({'name':name,'rejected_by':'layout assertions'});continue
 for n,(label,c,schedule) in enumerate(r.cases()):
  native=r.Scheduled(r.words,c,schedule);trace,out=native.run();actual,hout=host.run(c,schedule)
  if trace!=actual:
   results.append({'name':name,'rejected_by':label,'fixture_number':n,'difference':r.first_difference(trace,actual)})
   break
 else:raise AssertionError(('surviving source mutant',name))
result={'status':'ALL MUTANTS REJECTED','mutants':results,'target_compilations':0,'frozen_source_unchanged':hashlib.sha256((r.PACKET/'candidate.c').read_bytes()).hexdigest()==r.FROZEN}
(r.OUTPUT/'mutations.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
