exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/v1.py').read().split('V = {')[0])
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
B = R(HEAD + L_idx + TAIL, 's16 player', 's32 player')
H1 = 'static s8 *rdy(s32 p) { return &D_8010FFC4[p]; }\n'
HS = 'static Player84 *pst(s32 p) { return &D_80140420[p]; }\n'
HD = 'static Definitions64 *pdef(s32 c) { return &D_8010FD80[c]; }\n'
HE = 'static State20 *pex(s32 p) { return &D_80140640[p]; }\n'
HV = 'static States60 *pvals(s32 p) { return &D_801406C0[p]; }\n'
b1 = R(B, '&D_8010FFC4[player]', 'rdy(player)')
V = {}
V['st'] = H1 + HS + R(b1, '&D_80140420[player]', 'pst(player)')
V['def'] = H1 + HD + R(b1, '&D_8010FD80[vehicle->category]', 'pdef(vehicle->category)')
V['ex'] = H1 + HE + R(b1, '&D_80140640[player]', 'pex(player)')
V['st_def'] = H1 + HS + HD + R(R(b1, '&D_80140420[player]', 'pst(player)'), '&D_8010FD80[vehicle->category]', 'pdef(vehicle->category)')
V['st_ex'] = H1 + HS + HE + R(R(b1, '&D_80140420[player]', 'pst(player)'), '&D_80140640[player]', 'pex(player)')
V['def_ex'] = H1 + HD + HE + R(R(b1, '&D_8010FD80[vehicle->category]', 'pdef(vehicle->category)'), '&D_80140640[player]', 'pex(player)')
V['rdy_first'] = H1 + R(R(R(B, ' s8 *ready;\n', ''), ' ready=&D_8010FFC4[player];\n', ''), ' s32 player=vehicle->player;\n', ' s32 player=vehicle->player;\n s8 *ready;\n').replace(' state=&D_80140420', ' ready=rdy(player);\n state=&D_80140420')
