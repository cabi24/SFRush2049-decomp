exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/v1.py').read().split('V = {')[0])
B = HEAD.replace('s32 object','u32 object') + L_idx + TAIL
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
V = {}
V['cmp_ff'] = R(B, 'object!=-1', 'object!=0xFFFFFFFF')
V['s32cast'] = R(B, 'object!=-1', '(s32)object!=-1')
V['ready_early'] = R(R(B, ' s8 *ready;\n', ' s8 *ready=&D_8010FFC4[player];\n'), ' ready=&D_8010FFC4[player];\n', '')
V['noready'] = R(R(B, ' ready=&D_8010FFC4[player];\n', ''), '*ready=1', 'D_8010FFC4[player]=1')
V['obj_in_loop'] = R(R(B, ' u32 object;\n', ''), ' for(i=0;i<2;i++) {\n', ' for(i=0;i<2;i++) {\n  u32 object;\n')
V['s32obj_ff'] = R(HEAD + L_idx + TAIL, 'object!=-1', 'object!=0xFFFFFFFF')
V['u8neg'] = R(B, 'D_80140AE0[player]=-1;', 'D_80140AE0[player]=0xFFFFFFFF;')
