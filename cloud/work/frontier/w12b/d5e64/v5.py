import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/comp/d5e64.c').read()
m=re.search(r'^static s8 \*rdy.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
def R(s,a,b):
    assert a in s, a
    return s.replace(a,b)
V={}
V['use0']=R(b0,' ready=rdy(player);\n',' ready=rdy(player);\n *ready=0;\n')
V['use0bl']=R(b0,' for(i=0;',' *ready=0;\n for(i=0;')
