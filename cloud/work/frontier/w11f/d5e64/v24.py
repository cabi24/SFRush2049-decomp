b0 = open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/base_early.c').read().replace(' State20 *pair;\n', '')
H0 = 'static s8 *rdy(s32 p) { return &D_8010FFC4[p]; }\n'
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
b1 = R(b0, H0, '')
V = {}
V['rv'] = R(b1, 'rdy(player)', '&D_8010FFC4[vehicle->player]')
V['rv_noinit'] = R(R(R(b1, 'rdy(player)', '&D_8010FFC4[vehicle->player]'), ' s32 player=vehicle->player;\n', ' s32 player;\n'), ' ready=', ' player=vehicle->player;\n ready=')
V['rv_noinit2'] = R(R(R(b1, 'rdy(player)', '&D_8010FFC4[vehicle->player]'), ' s32 player=vehicle->player;\n', ' s32 player;\n'), ' state=&', ' player=vehicle->player;\n state=&')
V['pl_after'] = R(R(R(b1, 'rdy(player)', '&D_8010FFC4[player]'), ' s32 player=vehicle->player;\n', ' s32 player;\n'), ' ready=', ' player=vehicle->player;\n ready=')
