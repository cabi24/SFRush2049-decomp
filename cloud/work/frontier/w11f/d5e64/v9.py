exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/v1.py').read().split('V = {')[0])
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
B = R(HEAD + L_idx + TAIL, 's16 player', 's32 player')
b1 = R(B, '&D_8010FFC4[player]', 'rdy(player)')
V = {}
V['loc'] = 'static s8 *rdy(s32 p) { s8 *r = &D_8010FFC4[p]; return r; }\n' + b1
V['loc2'] = 'static s8 *rdy(s32 p) { s8 *r; r = &D_8010FFC4[p]; return r; }\n' + b1
V['plus'] = 'static s8 *rdy(s32 p) { return D_8010FFC4 + p; }\n' + b1
V['u32'] = 'static s8 *rdy(u32 p) { return &D_8010FFC4[p]; }\n' + b1
V['int'] = 'static s8 *rdy(int p) { return &D_8010FFC4[p]; }\n' + b1
V['pad1'] = 'static s8 *rdy(s32 p) { return &D_8010FFC4[p]; }\n' + R(b1, ' s32 object;\n', ' s32 object;\n s32 pad[4];\n')
