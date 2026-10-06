import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/h2.c').read()
m=re.search(r'^static s8 \*rdy.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()].replace(' s8 *ready;\n', ' f32 vec[3];\n s8 *ready;\n')
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
INIT = ''' ready=rdy(player);
 state=&D_80140420[player];
 state->definition=&D_8010FD80[vehicle->category];
'''
DD = ''' {
 s8 *r = rdy(player);
 %s
 ready = r;
 }
'''
V = {}
V['gidx'] = R(b0, INIT, DD % 'D_80140420[player].definition=&D_8010FD80[vehicle->category];\n state=&D_80140420[player];')
V['gidx2'] = R(b0, INIT, DD % 'state=&D_80140420[player];\n D_80140420[player].definition=&D_8010FD80[vehicle->category];')
V['gidx_b'] = R(b0, INIT, ' ready=rdy(player);\n D_80140420[player].definition=&D_8010FD80[vehicle->category];\n state=&D_80140420[player];\n')
V['loopidx'] = R(R(R(b0, INIT, DD % 'D_80140420[player].definition=&D_8010FD80[vehicle->category];'), 'state->', 'D_80140420[player].'), ' Player84 *state;\n', '')
