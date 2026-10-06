exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/v1.py').read().split('V = {')[0])
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
B = R(HEAD + L_idx + TAIL, 's16 player', 's32 player')
V = {}
V['vol'] = R(B, ' s8 *ready;', ' s8 * volatile ready;')
V['addr'] = R(B, ' *ready=1;', ' *ready=1;\n if(&ready){}')
