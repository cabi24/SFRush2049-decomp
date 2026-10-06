import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/h2.c').read()
m=re.search(r'^static s8 \*rdy.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
V = {}
V['nv'] = b0
V['pair_unused'] = R(b0, ' int i;\n', ' State20 *pair;\n int i;\n')
V['pair_used'] = R(R(b0, ' int i;\n', ' State20 *pair;\n int i;\n'), '  player_conditional_call(&state->pair[i]);\n', '  pair=&state->pair[i];\n  player_conditional_call(pair);\n')
V['def_local'] = R(R(b0, ' int i;\n', ' Definitions64 *def;\n int i;\n'), ' state->definition=&D_8010FD80[vehicle->category];\n', ' def=&D_8010FD80[vehicle->category];\n state->definition=def;\n')
V['vals_local'] = R(R(b0, ' int i;\n', ' States60 *vals;\n int i;\n'), '''  player_conditional_call(&D_801406C0[player].values[0]);
  player_conditional_call(&D_801406C0[player].values[1]);
  player_conditional_call(&D_801406C0[player].values[2]);''', '''  vals=&D_801406C0[player];
  player_conditional_call(&vals->values[0]);
  player_conditional_call(&vals->values[1]);
  player_conditional_call(&vals->values[2]);''')
