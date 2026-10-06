b0=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/d5e64/best_body.c').read()
def R(s,a,b):
    assert a in s, a
    return s.replace(a,b,1)
OLD=''' s32 player=vehicle->player;
 Player84 *state;
 int i;
 s32 object;
 object=(s32)&D_8010FFC4[player];
 state=&D_80140420[player];
'''
V={}
V['init']=R(b0,OLD,''' s32 player=vehicle->player;
 s32 object=(s32)&D_8010FFC4[player];
 Player84 *state=&D_80140420[player];
 int i;
''')
V['init2']=R(b0,OLD,''' s32 player=vehicle->player;
 Player84 *state;
 int i;
 s32 object=(s32)&D_8010FFC4[player];
 state=&D_80140420[player];
''')
