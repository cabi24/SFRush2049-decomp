import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/comp/mode.c').read()
m=re.search(r'^void func_800E05F0\(.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
V={}
V['base']=b0
b1=R(b0,'    s16 original_slot=model->index;\n    s16 slot;\n','    s32 original_slot=model->index;\n    s32 slot;\n')
V['s32']=b1
b2=R(R(b1,'    f32 style=0.5f,level=0.0f;\n','    f32 style,level;\n'),'    if (mode==2) {\n','    if (mode==2) {\n        style=0.5f; level=0.0f;\n')
V['s32_init']=b2
