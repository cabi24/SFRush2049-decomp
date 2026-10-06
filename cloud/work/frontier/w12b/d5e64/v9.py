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
TOP='  player_conditional_call(&state->pair[i]);\n'
V={}
for hn,h in [('h2',H2),('h0',H0)]:
  for vn,f in [('vec',lambda s:s),('novec',lambda s:R(s,' f32 vec[3];\n',''))]:
    s=f(R(noR,H2,h))
    V[hn+vn+'top']=R(s,TOP,'  ready=rdy(player);\n'+TOP)
    V[hn+vn+'aft']=R(s,TOP,TOP+'  ready=rdy(player);\n')
    V[hn+vn+'end']=R(s,'  if(object==-1) continue;\n','  ready=rdy(player);\n  if(object==-1) continue;\n')
