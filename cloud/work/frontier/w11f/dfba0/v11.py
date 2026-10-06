import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/dfba0/h4.c').read()
m=re.search(r'^void func_800DFBA0\(.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
V={}
def allc(s,f):
    for c in ['count0','count1','count2']: s=f(s,c)
    return s
V['pre_inc']=allc(b0,lambda s,c: R(R(s,'            %s++;\n'%c,''),'fraction/=%s==1'%c,'fraction/=++%s==1'%c))
V['plus1']=allc(b0,lambda s,c: R(s,'            %s++;\n'%c,'            %s=%s+1;\n'%(c,c)))
V['pluseq']=allc(b0,lambda s,c: R(s,'            %s++;\n'%c,'            %s+=1;\n'%c))
V['ne']=allc(b0,lambda s,c: R(s,'fraction/=%s==1 ? 2.0f : %s==2 ? 4.0f : 8.0f;'%(c,c),'fraction/=%s!=1 ? %s!=2 ? 8.0f : 4.0f : 2.0f;'%(c,c)))
V['cmp1first']=allc(b0,lambda s,c: R(s,'fraction/=%s==1 ? 2.0f : %s==2 ? 4.0f : 8.0f;'%(c,c),'fraction/=1==%s ? 2.0f : 2==%s ? 4.0f : 8.0f;'%(c,c)))
