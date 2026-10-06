import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/comp/d5e64.c').read()
m=re.search(r'^static s8 \*rdy.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
H2='static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }\n'
def R(s,a,b):
    assert a in s, a
    return s.replace(a,b)
noR=R(b0,' ready=rdy(player);\n','')
TOP='  player_conditional_call(&state->pair[i]);\n'
V={}
V['L2top']=R(noR,TOP,'  ready=rdy(player);\n'+TOP)
V['L1top']=R(R(noR,H2,''),TOP,'  ready=&D_8010FFC4[player];\n'+TOP)
V['L2mid']=R(noR,'  if(object==-1) continue;\n','  ready=rdy(player);\n  if(object==-1) continue;\n')
V['L1mid']=R(R(noR,H2,''),'  if(object==-1) continue;\n','  ready=&D_8010FFC4[player];\n  if(object==-1) continue;\n')
