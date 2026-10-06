import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/comp/d5e64.c').read()
m=re.search(r'^static s8 \*rdy.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
H2='static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }\n'
def R(s,a,b):
    assert a in s, a
    return s.replace(a,b)
nh=R(R(b0,H2,''),'rdy(player)','&D_8010FFC4[player]')
V={}
V['if1_r']=R(nh,' ready=&D_8010FFC4[player];\n',' if(1) ready=&D_8010FFC4[player];\n')
V['if1_s']=R(b0,' state=&D_80140420[player];\n',' if(1) state=&D_80140420[player];\n')
V['if1_rs']=R(V['if1_r'],' state=&D_80140420[player];\n',' if(1) state=&D_80140420[player];\n')
V['if1_rs_blk']=R(nh,' ready=&D_8010FFC4[player];\n state=&D_80140420[player];\n',' if(1) { ready=&D_8010FFC4[player];\n state=&D_80140420[player]; }\n')
