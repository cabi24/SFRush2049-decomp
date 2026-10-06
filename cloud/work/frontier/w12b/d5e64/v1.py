import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/comp/d5e64.c').read()
m=re.search(r'^static s8 \*rdy.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
H2='static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }\n'
def R(s,a,b):
    assert a in s, a
    return s.replace(a,b)
plain=R(R(b0,H2,''),'rdy(player)','&D_8010FFC4[player]')
V={}
V["base"]=b0
V['st_v']=R(b0,'state=&D_80140420[player]','state=&D_80140420[vehicle->player]')
V['rv_plain']=R(plain,'&D_8010FFC4[player]','&D_8010FFC4[vehicle->player]')
V['both_v']=R(V['rv_plain'],'state=&D_80140420[player]','state=&D_80140420[vehicle->player]')
V['both_v_h']=R(V['st_v'],'rdy(player)','rdy(vehicle->player)')
