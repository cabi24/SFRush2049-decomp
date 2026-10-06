import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/h2.c').read()
m=re.search(r'^static s8 \*rdy.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
H2='static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }\n'
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
plain = R(R(b0, H2, ''), 'rdy(player)', '&D_8010FFC4[player]')
V = {}
r2 = lambda s: R(s, ' ready=&D_8010FFC4[player];\n', ' ready=D_8010FFC4;\n ready+=player;\n')
s2 = lambda s: R(s, ' state=&D_80140420[player];\n', ' state=D_80140420;\n state+=player;\n')
vec = lambda s: R(s, ' s8 *ready;\n', ' f32 vec[3];\n s8 *ready;\n')
V['r2'] = r2(plain)
V['s2'] = s2(plain)
V['r2s2'] = s2(r2(plain))
V['r2s2v'] = vec(s2(r2(plain)))
V['r2v'] = vec(r2(plain))
V['s2_rdy2v'] = vec(s2(b0))
