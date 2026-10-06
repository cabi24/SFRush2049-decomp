import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/dfba0/h3.c').read()
m=re.search(r'^void func_800DFBA0\(.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
C1='''            if(fraction<0.0f) fraction=-fraction;
            fraction=fraction<0.0f ? 0.0f : fraction>1 ? 1 : fraction;
            fraction=1-(1-fraction)*(1-fraction);
'''
C2='''                if(fraction<0.0f) fraction=-fraction;
                fraction=fraction<0.0f ? 0.0f : fraction>1 ? 1 : fraction;
                fraction=1-(1-fraction)*(1-fraction);
'''
C3='''                if(fraction<0.0f)fraction=-fraction;
                fraction=fraction<0.0f ? 0.0f : fraction>1 ? 1 : fraction;
                fraction=1-(1-fraction)*(1-fraction);
'''
HC='''static f32 curve(f32 f)
{
    if(f<0.0f) f=-f;
    f=f<0.0f ? 0.0f : f>1 ? 1 : f;
    return 1-(1-f)*(1-f);
}
'''
HC2='''static f32 clampf(f32 f, f32 lo, f32 hi) { return f<lo ? lo : f>hi ? hi : f; }
static f32 curve(f32 f)
{
    if(f<0.0f) f=-f;
    f=clampf(f,0.0f,1);
    return 1-(1-f)*(1-f);
}
'''
HD='''static f32 divisor(s32 n) { return n==1 ? 2.0f : n==2 ? 4.0f : 8.0f; }
'''
def curve(s):
    s=R(s,C1,'            fraction=curve(fraction);\n')
    s=R(s,C2,'                fraction=curve(fraction);\n')
    s=R(s,C3,'                fraction=curve(fraction);\n')
    return s
def div(s):
    for c in ['count0','count1','count2']:
        s=R(s,'fraction/=%s==1 ? 2.0f : %s==2 ? 4.0f : 8.0f;'%(c,c),'fraction/=divisor(%s);'%c)
    return s
V={}
V['curve']=HC+curve(b0)
V['curve2']=HC2+curve(b0)
V['div']=HD+div(b0)
V['curve_div']=HC+HD+div(curve(b0))
V['curve2_div']=HC2+HD+div(curve(b0))
