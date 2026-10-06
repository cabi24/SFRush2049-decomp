import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/dfba0/h3.c').read()
m=re.search(r'^void func_800DFBA0\(.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
def sw(s, which):
    for c in which:
        s=R(s,'fraction/=%s==1 ? 2.0f : %s==2 ? 4.0f : 8.0f;'%(c,c),'switch(%s){case 1: fraction/=2.0f; break; case 2: fraction/=4.0f; break; default: fraction/=8.0f; break;}'%c)
    return s
def sw2(s, which):
    for c in which:
        s=R(s,'fraction/=%s==1 ? 2.0f : %s==2 ? 4.0f : 8.0f;'%(c,c),'switch(%s){case 1: scale=2.0f; break; case 2: scale=4.0f; break; default: scale=8.0f; break;}\n            fraction/=scale;'%c)
    return s
V={}
V['sw_all']=sw(b0,['count0','count1','count2'])
V['sw2_all']=sw2(b0,['count0','count1','count2'])
V['ifelse']=b0
for c in ['count0','count1','count2']:
    V['ifelse']=R(V['ifelse'],'fraction/=%s==1 ? 2.0f : %s==2 ? 4.0f : 8.0f;'%(c,c),'if(%s==1) fraction/=2.0f; else if(%s==2) fraction/=4.0f; else fraction/=8.0f;'%(c,c))
