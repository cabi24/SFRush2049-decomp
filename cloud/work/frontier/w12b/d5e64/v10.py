import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/comp/d5e64.c').read()
m=re.search(r'^static s8 \*rdy.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
H2='static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }\n'
H0='static s8 *rdy(s32 p) { return &D_8010FFC4[p]; }\n'
def R(s,a,b):
    assert a in s, a
    return s.replace(a,b)
noR=R(b0,' ready=rdy(player);\n','')
CHK=' if(!D_8010FFC0 || vehicle->disabled)return;\n'
V={}
for hn,h in [('h2',H2),('h0',H0),('pl',None)]:
    s=noR if h==H2 else (R(noR,H2,h) if h else R(noR,H2,''))
    e='rdy(player)' if h else '&D_8010FFC4[player]'
    V[hn+'mid']=R(s,CHK,' if(!D_8010FFC0)return;\n ready=%s;\n if(vehicle->disabled)return;\n'%e)
    V[hn+'pre']=R(s,CHK,' ready=%s;\n'%e+CHK)
