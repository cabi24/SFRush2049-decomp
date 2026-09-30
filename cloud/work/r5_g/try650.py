import sys,re,json,subprocess
sys.path.insert(0,'/home/user/SFRush2049-decomp/cloud/work/r5_g')
from gscore import *
G=ROOT+'/cloud/work/r5_g/grp2'
S=open(G+'/group.c').read()
m=re.search(r'void func_800AD650\(f32 \*o, s16 \*p\) \{.*?\n\}\n',S,re.S)
def t(body):
    open(G+'/group.c','w').write(S.replace(m.group(0),body))
    return zb(G,'func_800AD650')
V={}
V['a']='''void func_800AD650(f32 *o, s16 *p) {
    o[0] = (f32) p[0] * 0.00006103515625f;
    o[1] = (f32) p[1] * 0.00006103515625f;
    o[2] = (f32) p[2] * 0.00006103515625f;
    o[3] = (f32) p[3] * 0.00006103515625f;
    o[4] = (f32) p[4] * 0.00006103515625f;
    o[5] = (f32) p[5] * 0.00006103515625f;
    o[6] = (f32) p[6] * 0.00006103515625f;
    o[7] = (f32) p[7] * 0.00006103515625f;
    o[8] = (f32) p[8] * 0.00006103515625f;
}
'''
V['b']=V['a'].replace('0.00006103515625f','(1.0f / 16384.0f)')
V['c']=V['a'].replace('(f32) p[','(f32)(s32) p[')
V['d']='''void func_800AD650(f32 *o, s16 *p) {
    s32 i;
    for (i = 0; i < 9; i++) {
        o[i] = (f32) p[i] * 0.00006103515625f;
    }
}
'''.replace('s32 i;','volatile s32 i;')
V['e']=V['a'].replace('void func_800AD650(f32 *o, s16 *p) {','void func_800AD650(f32 *o, s16 *p) {\n    f32 k = 0.00006103515625f;').replace('0.00006103515625f;\n    o','k;\n    o').replace('* 0.00006103515625f','* k')
for k,v in V.items(): print(k,t(v))
