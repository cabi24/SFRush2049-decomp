import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/dfba0/h3.c').read()
m=re.search(r'^void func_800DFBA0\(.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
D='    s32 i,count0,count1,count2;\n'
b1=R(b0,D,'    s32 i;\n    u32 count0,count1,count2;\n')
V={}
V['u_all']=R(R(R(b1,'contact[i]==1','contact[i]==1u'),'contact[i]==2','contact[i]==2u'),'contact[i]==3','contact[i]==3u')
V['u_12']=R(R(b1,'contact[i]==1','contact[i]==1u'),'contact[i]==2','contact[i]==2u')
V['u_all0']=R(V['u_all'],'contact[i]==0','contact[i]==0u')
