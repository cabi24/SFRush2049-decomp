b0 = open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/base_early.c').read().replace(' State20 *pair;\n', '')
H0 = 'static s8 *rdy(s32 p) { return &D_8010FFC4[p]; }'
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
INIT = ''' ready=rdy(player);
 state=&D_80140420[player];
 state->definition=&D_8010FD80[vehicle->category];
'''
D = ''' {
 s8 *r = rdy(player);
 state=&D_80140420[player];
 state->definition=&D_8010FD80[vehicle->category];
 ready = r;
 }
'''
V = {}
V['d_h0'] = R(b0, INIT, D)
V['d_hr'] = R(R(b0, INIT, D), H0, 'static s8 *rdy(s32 p) { s8 *r = &D_8010FFC4[p]; return r; }')
V['d_noblk'] = R(R(R(b0, INIT, D.replace(' {\n s8 *r', ' r').replace(' }\n','')), ' s8 *ready;\n', ' s8 *ready;\n s8 *r;\n'), H0, 'static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }')
V['d_noblk_h0'] = R(R(b0, INIT, D.replace(' {\n s8 *r', ' r').replace(' }\n','')), ' s8 *ready;\n', ' s8 *ready;\n s8 *r;\n')
V['d_vol'] = R(R(R(b0, INIT, D.replace('rdy(player)', '&D_8010FFC4[player]')), H0, ''), ' s8 *r', ' s8 * volatile r')
