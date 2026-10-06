b0 = open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/base_early.c').read().replace(' State20 *pair;\n', '')
H0 = 'static s8 *rdy(s32 p) { return &D_8010FFC4[p]; }'
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
ad = R(R(b0, ' ready=rdy(player);\n', ''), ' for(i=0;', ' ready=rdy(player);\n for(i=0;')
V = {}
V['ad_h0'] = ad
V['ad_hr'] = R(ad, H0, 'static s8 *rdy(s32 p) { s8 *r = &D_8010FFC4[p]; return r; }')
V['ad_plain'] = R(R(ad, H0, ''), 'rdy(player)', '&D_8010FFC4[player]')
V['ad_vol'] = R(R(R(ad, H0, ''), 'rdy(player)', '&D_8010FFC4[player]'), ' s8 *ready;', ' s8 * volatile ready;')
V['early_ptr'] = R(R(b0, ' s8 *ready;\n', ' s8 *ready;\n s8 *r;\n'), ' ready=rdy(player);\n', ' r=&D_8010FFC4[player];\n').replace(' for(i=0;', ' ready=r;\n for(i=0;')
