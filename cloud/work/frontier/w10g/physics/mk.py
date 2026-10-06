import sys
d='physics/'
s0=open(d+'g0.c').read()
old='''void func_8008B640(s32 idx, f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz) {
    f32 *v;

    v = *(f32 **) ((u8 *) &D_8012E708 + ((s16) idx * 0x44));'''
o2='m = *(f32 **) ((u8 *) &D_8012E708 + (ipa_s1->model * 0x44));'
TY='typedef struct { f32 *m; u8 pad4[0x40]; } ModelSlot;\n#define TBL ((ModelSlot *) &D_8012E708)\n'
V={
'a': (TY, 'v = TBL[(s16) idx].m;', 'm = TBL[ipa_s1->model].m;'),
'b': (TY+'static ModelSlot *model_slot(s16 idx) { return &TBL[idx]; }\n', 'v = model_slot(idx)->m;', 'm = model_slot(ipa_s1->model)->m;'),
'c': (TY+'static f32 *model_mat(s16 idx) { return TBL[idx].m; }\n', 'v = model_mat(idx);', 'm = model_mat(ipa_s1->model);'),
'd': (TY+'static f32 *model_mat(s32 idx) { return TBL[(s16) idx].m; }\n', 'v = model_mat(idx);', 'm = model_mat(ipa_s1->model);'),
'e': (TY+'static f32 *model_mat(s16 idx) { f32 *p = TBL[idx].m; return p; }\n', 'v = model_mat(idx);', 'm = model_mat(ipa_s1->model);'),
}
for k,(pre,a,b) in V.items():
    s=s0.replace(old, pre+'''void func_8008B640(s32 idx, f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz) {
    f32 *v;

    '''+a).replace(o2,b)
    open(d+'h_%s.c'%k,'w').write(s)
