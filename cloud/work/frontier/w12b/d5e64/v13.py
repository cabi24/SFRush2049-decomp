import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/comp/d5e64.c').read()
m=re.search(r'^static s8 \*rdy.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
H2='static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }\n'
def R(s,a,b):
    assert a in s, a
    return s.replace(a,b)
CHK=' if(!D_8010FFC0 || vehicle->disabled)return;\n'
ST=' state=&D_80140420[player];\n'
noS=R(b0,ST,'')
V={}
V['s_pre']=R(noS,CHK,ST+CHK)
V['s_mid']=R(noS,CHK,' if(!D_8010FFC0)return;\n'+ST+' if(vehicle->disabled)return;\n')
plain=R(R(noS,H2,''),' ready=rdy(player);\n','')
V['sr_pre']=R(plain,CHK,ST+' ready=&D_8010FFC4[player];\n'+CHK)
V['rs_pre']=R(plain,CHK,' ready=&D_8010FFC4[player];\n'+ST+CHK)
V['s_pre_r_plain']=R(plain,CHK,ST+CHK+' ready=&D_8010FFC4[player];\n')
V['decl_init']=R(R(plain,' Player84 *state;\n',' Player84 *state=&D_80140420[player];\n'),' s8 *ready;\n',' s8 *ready=&D_8010FFC4[player];\n')
