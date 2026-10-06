import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/comp/d5e64.c').read()
m=re.search(r'^static s8 \*rdy.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
def R(s,a,b):
    assert a in s, a
    return s.replace(a,b)
A='state=&D_80140420[player]'
V={}
V['S1']=R(b0,A,'state=D_80140420+player')
V['S2']=R(b0,A,'state=(Player84*)((u32)D_80140420+player*sizeof(Player84))')
V['S3']=R(b0,A,'state=(Player84*)((u32)&D_80140420[player])')
V['S4']=R(b0,A,'state=(Player84*)((char*)D_80140420+player*84)')
