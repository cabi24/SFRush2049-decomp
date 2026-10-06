b0 = open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/base_early.c').read().replace(' State20 *pair;\n', '')
H0 = 'static s8 *rdy(s32 p) { return &D_8010FFC4[p]; }'
H2 = 'static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }'
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
pre = R(R(b0, ' ready=rdy(player);\n', ''), ' if(!D_8010FFC0', ' ready=rdy(player);\n if(!D_8010FFC0')
V = {}
V['pre_h2'] = R(pre, H0, H2)
V['pre_plain'] = R(R(pre, H0, ''), 'rdy(player)', '&D_8010FFC4[player]')
V['pre_h0'] = pre
