import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/dfba0/h3.c').read()
m=re.search(r'^void func_800DFBA0\(.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
D='    s32 i,count0,count1,count2;\n'
V={}
V['u32c']=R(b0,D,'    s32 i;\n    u32 count0,count1,count2;\n')
V['u32all']=R(b0,D,'    u32 i,count0,count1,count2;\n')
V['u32i']=R(b0,D,'    u32 i;\n    s32 count0,count1,count2;\n')
V['u16contact']=R(b0,'model->contact[i]==1','model->contact[i]==1u')
