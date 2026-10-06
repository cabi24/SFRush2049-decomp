exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/v1.py').read().split('V = {')[0])
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
B = R(HEAD + L_idx + TAIL, 's16 player', 's32 player')
V = {}
V['rv'] = R(B, ' ready=&D_8010FFC4[player];\n', ' ready=&D_8010FFC4[vehicle->player];\n')
V['rv2'] = R(R(B, ' ready=&D_8010FFC4[player];\n', ''), ' state=&D_80140420[player];\n', ' state=&D_80140420[player];\n ready=&D_8010FFC4[vehicle->player];\n')
V['rv3'] = R(R(B, ' ready=&D_8010FFC4[player];\n', ''), ' state->definition=&D_8010FD80[vehicle->category];\n', ' state->definition=&D_8010FD80[vehicle->category];\n ready=&D_8010FFC4[vehicle->player];\n')
