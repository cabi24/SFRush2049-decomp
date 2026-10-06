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
RD=' ready=rdy(player);\n'
base=R(R(b0,ST,''),CHK,ST+CHK)   # s_pre
plain=R(R(base,H2,''),RD,' ready=&D_8010FFC4[player];\n')
V={}
for k,g in [('bare','if(ready);'),('ne0','if(ready!=0);'),('deref','if(*ready);'),('cast','if((s32)ready);')]:
    V['p_'+k]=R(plain,' ready=&D_8010FFC4[player];\n',' ready=&D_8010FFC4[player];\n '+g+'\n')
    V['pb_'+k]=R(plain,' for(i=0;',' '+g+'\n for(i=0;')
V2={}
for k in ['p_bare','p_ne0','p_deref']:
    V2[k+'_novec']=R(V[k],' f32 vec[3];\n','')
V=V2
