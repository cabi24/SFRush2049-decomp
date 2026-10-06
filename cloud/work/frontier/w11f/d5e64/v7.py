exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/v1.py').read().split('V = {')[0])
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
B = R(HEAD + L_idx + TAIL, 's16 player', 's32 player')
V = {}
H1 = 'static s8 *rdy(s32 p) { return &D_8010FFC4[p]; }\n'
H2 = 'static s8 *rdy(s16 p) { return &D_8010FFC4[p]; }\n'
H3 = 'static s8 *rdy(Vehicle2056 *v) { return &D_8010FFC4[v->player]; }\n'
V['h1'] = H1 + R(B, '&D_8010FFC4[player]', 'rdy(player)')
V['h2'] = H2 + R(B, '&D_8010FFC4[player]', 'rdy(player)')
V['h3'] = H3 + R(B, '&D_8010FFC4[player]', 'rdy(vehicle)')
V['h1v'] = H1 + R(R(B, '&D_8010FFC4[player]', 'rdy(player)'), ' s8 *ready;', ' s8 * volatile ready;')
