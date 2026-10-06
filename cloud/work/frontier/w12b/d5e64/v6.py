import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/comp/d5e64.c').read()
m=re.search(r'^static s8 \*rdy.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
H2='static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }\n'
def R(s,a,b):
    assert a in s, a
    return s.replace(a,b)
nh=R(b0,H2,'')
V={}
V['r2']=R(nh,' ready=rdy(player);\n',' ready=D_8010FFC4;\n ready+=player;\n')
V['r2b']=R(nh,' ready=rdy(player);\n',' ready=D_8010FFC4;\n ready=ready+player;\n')
V['r2c']=R(nh,' ready=rdy(player);\n',' ready=D_8010FFC4;\n ready=&ready[player];\n')
V['r2_s2']=R(V['r2'],' state=&D_80140420[player];\n',' state=D_80140420;\n state+=player;\n')
V['h_s2']=R(b0,' state=&D_80140420[player];\n',' state=D_80140420;\n state+=player;\n')
