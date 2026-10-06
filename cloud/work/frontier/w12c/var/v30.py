import re
src=open('msh_d.c').read()
def cases(fn):
    out=[]
    for a,b in [('0.65f, 1.0f, 0.0f','0.65f, X1, X0'),('0.65f, -1.0f, 0.0f','0.65f, XM1, X0'),('0.65f, 0.0f, 0.0f','0.65f, X0, X0')]:
        pass
    return out
def mk(one,mone,zero):
    reps=[]
    s=src
    n=src.count('0.65f, 1.0f, 0.0f')
    return [('0.65f, 1.0f, 0.0f);','0.65f, %s, %s);'%(one,zero)),('0.65f, 1.0f, 0.0f);','0.65f, %s, %s);'%(one,zero)),
            ('0.65f, -1.0f, 0.0f);','0.65f, %s, %s);'%(mone,zero)),('0.65f, -1.0f, 0.0f);','0.65f, %s, %s);'%(mone,zero)),
            ('0.65f, 0.0f, 0.0f);','0.65f, %s, %s);'%(zero,zero)),('0.65f, 0.0f, 0.0f);','0.65f, %s, %s);'%(zero,zero))]
V={'i1':mk('1','-1','0.0f'),'i1i0':mk('1','-1','0'),'d1':mk('1.0','-1.0','0.0f')}
