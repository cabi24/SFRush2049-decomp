import sys
# mkh.py OUT [inner_sig] : extract E05F0's first MODE==2 block into func_800E0048 defined before E0050
s=open('comp/mode.c').read()
a=s.index('void func_800E05F0(ModelView *model)')
b=s.index('    if (MODE(model)==2) {\n        style=0.5f;',a)
e=s.index('    if(MODE(model)!=2)return;',b)
block=s[b:e]
body=s[a:]
decl='''    s32 slot;
    LayerState *entry;
    f32 style,level;
    f32 weighted,value;
    s32 i;
'''
helper='void func_800E0048(ModelView *model)\n{\n'+decl+'    slot=model->index;\n'+block+'}\n'
newbody=body.replace(block,'    func_800E0048(model);\n').replace(decl,'',1).replace('    slot=model->index;\n','',1)
pre=s[:a]
m=pre.index('static void layer_set')
statics=pre[m:]
pre=pre[:m]
k=pre.index('void func_800E0050(MODELDAT *m) {')
c=statics.index('/*\n * func_800E05F0')
statics=statics[:c]
mdef='#define MODE(m) FIELD(m,s8,1996)\n'
out=pre[:k]+statics+mdef+helper+pre[k:]+newbody.replace(mdef,'')
open(sys.argv[1],'w').write(out)
