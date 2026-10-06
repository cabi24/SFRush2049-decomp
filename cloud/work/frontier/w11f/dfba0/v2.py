import re, itertools
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/dfba0/h1.c').read()
m=re.search(r'^void func_800DFBA0\(.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
loop_start=b0.index('    for (i=0;i<4;i++)'); loop_end=b0.index('    if(sum0<sum1')
def ints(s, one, zero, div):
    L=s[loop_start:loop_end]
    if one:
        L=L.replace('1.0f-','1-').replace('>1.0f ? 1.0f','>1 ? 1')
    if zero:
        L=L.replace('<0.0f ? 0.0f','<0 ? 0').replace('fraction<0.0f) fraction','fraction<0) fraction').replace('fraction<0.0f)fraction','fraction<0)fraction').replace('!=0.0f','!=0')
    if div:
        L=L.replace('2.0f : ','2 : ').replace('4.0f : 8.0f','4 : 8')
    return s[:loop_start]+L+s[loop_end:]
V={}
for o,z,d in itertools.product([0,1],[0,1],[0,1]):
    V['i%d%d%d'%(o,z,d)]=ints(b0,o,z,d)
