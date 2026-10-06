exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/v1.py').read().split('V = {')[0])
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
B = R(HEAD + L_idx + TAIL, 's16 player', 's32 player')
V = {}
V['base'] = B
V['ready_pre'] = R(R(B, ' ready=&D_8010FFC4[player];\n', ''), ' if(!D_8010FFC0', ' ready=&D_8010FFC4[player];\n if(!D_8010FFC0')
V['ready_init'] = R(R(B, ' s8 *ready;\n', ' s8 *ready=&D_8010FFC4[vehicle->player];\n'), ' ready=&D_8010FFC4[player];\n', '')
V['ready_first'] = R(R(R(B, ' s8 *ready;\n', ''), ' ready=&D_8010FFC4[player];\n', ''), ' s32 player=vehicle->player;\n', ' s32 player=vehicle->player;\n s8 *ready=&D_8010FFC4[player];\n')
V['ready_after_state'] = R(R(B, ' ready=&D_8010FFC4[player];\n', ''), ' state->definition=', ' ready=&D_8010FFC4[player];\n state->definition=')
V['ready_plus'] = R(B, '&D_8010FFC4[player]', 'D_8010FFC4+player')
V['ready_u'] = B.replace('ready=&D_8010FFC4[player]', 'ready=(s8*)(u32)&D_8010FFC4[player]')
