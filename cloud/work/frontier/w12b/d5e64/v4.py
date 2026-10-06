import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/comp/d5e64.c').read()
m=re.search(r'^static s8 \*rdy.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
def R(s,a,b):
    assert a in s, a
    return s.replace(a,b)
V={}
for k,g in [('bare','if(ready);'),('ne0','if(ready!=0);'),('cast','if((s32)ready);'),('deref','if(*ready);'),('derefne','if(*ready!=0);'),('ult','if((u32)ready<1);')]:
    V['ad_'+k]=R(b0,' ready=rdy(player);\n',' ready=rdy(player);\n '+g+'\n')
    V['bl_'+k]=R(b0,' for(i=0;',' '+g+'\n for(i=0;')
