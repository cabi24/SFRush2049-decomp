b0 = open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/base_early.c').read().replace(' State20 *pair;\n', '').replace('static s8 *rdy(s32 p) { return &D_8010FFC4[p]; }', 'static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }')
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
V = {}
V['base'] = b0
V['afterdef'] = R(R(b0, ' ready=rdy(player);\n', ''), ' for(i=0;', ' ready=rdy(player);\n for(i=0;')
V['afterst'] = R(R(b0, ' ready=rdy(player);\n', ''), ' state->definition=', ' ready=rdy(player);\n state->definition=')
V['h_r'] = R(b0, 'static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }', 'static s8 *rdy(s32 p) { s8 *r = &D_8010FFC4[p]; return r; }')
