import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/comp/d5e64.c').read()
m=re.search(r'^static s8 \*rdy.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
H2='static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }\n'
H0='static s8 *rdy(s32 p) { return &D_8010FFC4[p]; }\n'
def R(s,a,b):
    assert a in s, a
    return s.replace(a,b)
CHK=' if(!D_8010FFC0 || vehicle->disabled)return;\n'
ST=' state=&D_80140420[player];\n'
RD=' ready=rdy(player);\n'
base=R(R(b0,ST,''),CHK,ST+CHK)   # s_pre
nr=R(base,RD,'')
V={}
V['h2_pre']=R(nr,CHK,RD+CHK)
V['h2_pre_first']=R(nr,ST+CHK,RD+ST+CHK)
V['h0_here']=R(base,H2,H0)
V['h0_pre']=R(R(nr,H2,H0),CHK,RD+CHK)
V['h2_after_def']=R(nr,' for(i=0;',RD+' for(i=0;')
V['h2_after_st_def']=R(nr,' state->definition=',RD+' state->definition=')
