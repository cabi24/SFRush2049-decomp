exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/v8.py').read().split('V = {}')[0])
def A(s): return R(s, ' s8 *ready;\n', ' f32 vec[3];\n s8 *ready;\n')
b0 = A(R(B, '&D_8010FFC4[player]', 'rdy(player)'))
V = {}
V['void'] = 'static void *rdy(s32 p) { return &D_8010FFC4[p]; }\n' + b0
V['u32'] = 'static u32 rdy(s32 p) { return (u32)&D_8010FFC4[p]; }\n' + R(b0, 'rdy(player)', '(s8*)rdy(player)')
V['nest'] = 'static s8 *rdy0(s32 p) { return &D_8010FFC4[p]; }\nstatic s8 *rdy(s32 p) { return rdy0(p); }\n' + b0
V['two'] = 'static s8 *rdy(s32 p) { s8 *r = &D_8010FFC4[p]; *r = 0; return r; }\n' + b0
V['vp'] = 'static s8 *rdy(Vehicle2056 *v) { s32 p = v->player; return &D_8010FFC4[p]; }\n' + R(b0, 'rdy(player)', 'rdy(vehicle)')
V['late'] = H1 + R(R(b0, ' ready=rdy(player);\n', ''), ' for(i=0;i<2;i++)', ' ready=rdy(player);\n for(i=0;i<2;i++)')
V['late2'] = H1 + R(R(b0, ' ready=rdy(player);\n', ''), ' state->definition=', ' ready=rdy(player);\n state->definition=')
