import sys,re,itertools;sys.path.insert(0,'/home/user/SFRush2049-decomp/cloud/work/r5_g')
from ipcsub import *
V2=open(ROOT+'/cloud/work/r5_g/ipc_v2.c').read()
D0=V2[V2.index('    u16 idx[20];'):V2.index('    res = 1;')]
TYPES={'idx':'u16 idx[20];','vp':'f32 vp[3];','vq':'f32 vq[3];','va':'f32 va[3];','ve':'f32 ve[3];','v0':'f32 v0[3];','vprev':'f32 vprev[3];','vd':'f32 vd[3];','f1':'volatile f32 f1;','f2':'volatile f32 f2;','k':'u32 k;','n':'u32 n;','res':'s32 res;','t':'f32 t;','d':'f32 d;','e':'PV *e;'}
def build(order, extra=''):
    return V2.replace(D0,''.join('    '+TYPES[x]+'\n' for x in order)+extra+'\n')
def trial(order):
    put(build(order),'input_process_controller')
    return frame(), run(GD,'input_process_controller')[0]
