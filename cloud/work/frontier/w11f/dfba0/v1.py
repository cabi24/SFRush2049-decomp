import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/dfba0/h0.c').read()
m=re.search(r'^void func_800DFBA0\(.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
A0='''            if(count0==1) fraction/=2.0f;
            else fraction/=count0==2 ? 4.0f : 8.0f;'''
A1='''            if(count1==1)fraction/=2.0f;
            else fraction/=count1==2 ? 4.0f : 8.0f;'''
A2='''            scale=count2==1 ? 2.0f : count2==2 ? 4.0f : 8.0f;
            fraction/=scale;'''
def tern(c): return '            fraction/=%s==1 ? 2.0f : %s==2 ? 4.0f : 8.0f;' % (c,c)
def sc(c): return '            scale=%s==1 ? 2.0f : %s==2 ? 4.0f : 8.0f;\n            fraction/=scale;' % (c,c)
V={}
V['base']=b0
V['tern_all']=R(R(R(b0,A0,tern('count0')),A1,tern('count1')),A2,tern('count2'))
V['sc_all']=R(R(R(b0,A0,sc('count0')),A1,sc('count1')),A2,sc('count2'))
V['tern12_sc3']=R(R(b0,A0,tern('count0')),A1,tern('count1'))
