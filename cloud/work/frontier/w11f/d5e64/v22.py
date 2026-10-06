b0 = open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/base_early.c').read().replace(' State20 *pair;\n', '')
H0 = 'static s8 *rdy(s32 p) { return &D_8010FFC4[p]; }'
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
INIT = ''' ready=rdy(player);
 state=&D_80140420[player];
 state->definition=&D_8010FD80[vehicle->category];
'''
V = {}
V['h_all'] = R(R(b0, H0, '''static s8 *rdy(Vehicle2056 *v, s32 p) { s8 *r = &D_8010FFC4[p]; D_80140420[p].definition = &D_8010FD80[v->category]; return r; }'''), INIT, ' ready=rdy(vehicle,player);\n state=&D_80140420[player];\n')
V['h_all2'] = R(R(b0, H0, '''static s8 *rdy(Vehicle2056 *v, s32 p, Player84 *st) { s8 *r = &D_8010FFC4[p]; st->definition = &D_8010FD80[v->category]; return r; }'''), INIT, ' state=&D_80140420[player];\n ready=rdy(vehicle,player,state);\n')
V['h_all3'] = R(R(b0, H0, '''static s8 *rdy(Vehicle2056 *v, s32 p, Player84 *st) { s8 *r; r = &D_8010FFC4[p]; st->definition = &D_8010FD80[v->category]; return r; }'''), INIT, ' state=&D_80140420[player];\n ready=rdy(vehicle,player,state);\n')
V['h_all4'] = R(R(b0, H0, '''static s8 *rdy(s32 p, Player84 *st, s32 c) { s8 *r = &D_8010FFC4[p]; st->definition = &D_8010FD80[c]; return r; }'''), INIT, ' state=&D_80140420[player];\n ready=rdy(player,state,vehicle->category);\n')
