import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/dfba0/h4.c').read()
m=re.search(r'^void func_800DFBA0\(.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
V={}
V['probe']=R(b0,'    slot=model->index;\n','    slot=model->index;\n    if (slot == 1) slot = 2;\n')
