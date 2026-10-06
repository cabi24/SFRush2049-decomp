b0 = open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/base_early.c').read().replace(' State20 *pair;\n', '')
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
V = {}
V['pre'] = R(R(b0, ' ready=rdy(player);\n', ''), ' if(!D_8010FFC0', ' ready=rdy(player);\n if(!D_8010FFC0')
V['init'] = R(R(b0, ' ready=rdy(player);\n', ''), ' s8 *ready;\n', ' s8 *ready=rdy(player);\n')
V['afterdef'] = R(R(b0, ' ready=rdy(player);\n', ''), ' for(i=0;', ' ready=rdy(player);\n for(i=0;')
V['iearly'] = R(R(b0, 'for(i=0;i<2;i++)', 'for(;i<2;i++)'), ' ready=rdy(player);\n', ' i=0;\n ready=rdy(player);\n')
V['while'] = R(R(b0, 'for(i=0;i<2;i++) {', 'i=0;\n do {'), '  }\n }\n player_conditional_call(&state->extra)', '  }\n } while(++i<2);\n player_conditional_call(&state->extra)')
V['rdy2'] = R(b0, 'static s8 *rdy(s32 p) { return &D_8010FFC4[p]; }', 'static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }')
