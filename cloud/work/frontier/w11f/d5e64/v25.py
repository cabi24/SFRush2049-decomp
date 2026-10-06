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
V['a'] = R(R(b0, H0, '''static s8 *rdy(s32 p, Player84 *st, s32 c) { s8 *r; if(1) r = &D_8010FFC4[p]; st->definition = &D_8010FD80[c]; return r; }'''), INIT, ' state=&D_80140420[player];\n ready=rdy(player,state,vehicle->category);\n')
V['b'] = R(R(b0, H0, '''static s8 *rdy(s32 p, Player84 *st, Vehicle2056 *v) { s8 *r; if(1) r = &D_8010FFC4[p]; st->definition = &D_8010FD80[v->category]; return r; }'''), INIT, ' state=&D_80140420[player];\n ready=rdy(player,state,vehicle);\n')
V['c'] = R(R(b0, H0, '''static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }'''), INIT, ''' ready=rdy(player);
 state=&D_80140420[player];
 if(1) state->definition=&D_8010FD80[vehicle->category];
''')
V['d'] = R(R(b0, H0, '''static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }'''), INIT, ''' {
 s8 *r = rdy(player);
 state=&D_80140420[player];
 state->definition=&D_8010FD80[vehicle->category];
 ready = r;
 }
''')
V['e'] = R(R(b0, H0, ''), INIT, ''' {
 s8 *r;
 if(1) r = &D_8010FFC4[player];
 state=&D_80140420[player];
 state->definition=&D_8010FD80[vehicle->category];
 ready = r;
 }
''')
