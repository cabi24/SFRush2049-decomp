b0 = open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/base_early.c').read()
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
V = {}
V['nopair'] = R(b0, ' State20 *pair;\n', '')
V['plain'] = R(R(b0, 'static s8 *rdy(s32 p) { return &D_8010FFC4[p]; }\n', ''), 'rdy(player)', '&D_8010FFC4[player]')
V['plainvol'] = R(R(R(b0, 'static s8 *rdy(s32 p) { return &D_8010FFC4[p]; }\n', ''), 'rdy(player)', '&D_8010FFC4[player]'), ' s8 *ready;', ' s8 * volatile ready;')
V['i_first'] = R(R(b0, ' int i;\n', ''), ' s32 player=', ' int i;\n s32 player=')
V['ready_last'] = R(R(b0, ' s8 *ready;\n', ''), ' s32 object;\n', ' s32 object;\n s8 *ready;\n')
V['s16i'] = R(b0, ' int i;', ' s16 i;')
V['vecinh'] = R(R(b0, ' f32 vec[3];\n', ''), 'static s8 *rdy(s32 p) { return &D_8010FFC4[p]; }', 'static s8 *rdy(s32 p) { f32 vec[3]; return &D_8010FFC4[p]; }')
