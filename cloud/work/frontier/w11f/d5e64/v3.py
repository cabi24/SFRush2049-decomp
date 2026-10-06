exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/v1.py').read().split('V = {')[0])
B = HEAD.replace('s32 object','u32 object') + L_idx + TAIL
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
V = {}
V['p32'] = R(B, 's16 player', 's32 player')
V['p32s'] = R(HEAD + L_idx + TAIL, 's16 player', 's32 player')
V['p32m'] = R(HEAD + L_mix + TAIL, 's16 player', 's32 player')
V['p32mu'] = R(HEAD.replace('s32 object','u32 object') + L_mix + TAIL, 's16 player', 's32 player')
