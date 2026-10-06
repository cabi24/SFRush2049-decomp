import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/h2.c').read()
m=re.search(r'^static s8 \*rdy.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()].replace(' s8 *ready;\n', ' f32 vec[3];\n s8 *ready;\n')
H2='static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }\n'
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
plain = R(R(b0, H2, ''), 'rdy(player)', '&D_8010FFC4[player]')
D = lambda s: R(s, ''' ready=rdy(player);
 state=&D_80140420[player];
 state->definition=&D_8010FD80[vehicle->category];
''', ''' {
 s8 *r = rdy(player);
 state=&D_80140420[player];
 state->definition=&D_8010FD80[vehicle->category];
 ready = r;
 }
''')
readys = {'rdy2': b0, 'plain': plain, 'd': D(b0)}
states = {
 'u32': lambda s: R(s, 'state=&D_80140420[player];', 'state=(Player84 *)(u32)&D_80140420[player];'),
 'hs': lambda s: 'static Player84 *pst(s32 p) { return &D_80140420[p]; }\n' + R(s, 'state=&D_80140420[player];', 'state=pst(player);'),
 'hs2': lambda s: 'static Player84 *pst(s32 p) { Player84 *q; if(1) q = &D_80140420[p]; return q; }\n' + R(s, 'state=&D_80140420[player];', 'state=pst(player);'),
}
V = {}
for rk, rv in readys.items():
    for sk, sf in states.items():
        V[rk+'_'+sk] = sf(rv)
