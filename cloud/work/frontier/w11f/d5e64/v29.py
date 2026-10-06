import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/h2.c').read()
m=re.search(r'^static s8 \*rdy.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()].replace(' s8 *ready;\n', ' f32 vec[3];\n s8 *ready;\n')
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
INIT = ''' ready=rdy(player);
 state=&D_80140420[player];
 state->definition=&D_8010FD80[vehicle->category];
 for(i=0;i<2;i++) {
'''
V = {}
V['forinit'] = R(R(b0, INIT, ''' {
 s8 *r = rdy(player);
 state=&D_80140420[player];
 state->definition=&D_8010FD80[vehicle->category];
 for(ready=r,i=0;i<2;i++) {
'''), '\n }\n player_conditional_call(&state->extra);', '\n }\n }\n player_conditional_call(&state->extra);')
V['forinit2'] = R(R(b0, INIT, ''' {
 s8 *r = rdy(player);
 state=&D_80140420[player];
 state->definition=&D_8010FD80[vehicle->category];
 for(i=0,ready=r;i<2;i++) {
'''), '\n }\n player_conditional_call(&state->extra);', '\n }\n }\n player_conditional_call(&state->extra);')
V['sameline'] = R(R(b0, INIT, ''' {
 s8 *r = rdy(player);
 state=&D_80140420[player];
 state->definition=&D_8010FD80[vehicle->category];
 ready = r; for(i=0;i<2;i++) {
'''), '\n }\n player_conditional_call(&state->extra);', '\n }\n }\n player_conditional_call(&state->extra);')
V['rplain_forinit'] = R(R(R(b0, INIT, ''' {
 s8 *r = rdy(player);
 state=&D_80140420[player];
 state->definition=&D_8010FD80[vehicle->category];
 for(ready=r,i=0;i<2;i++) {
'''), '\n }\n player_conditional_call(&state->extra);', '\n }\n }\n player_conditional_call(&state->extra);'), 'static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }', 'static s8 *rdy(s32 p) { return &D_8010FFC4[p]; }')
