exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/v10.py').read().split('V = {}')[0])
V = {}
for t in ['s16']:
    V['trio_'+t] = H1 + 'static void trio(%s p) {\n  player_conditional_call(&D_801406C0[p].values[0]);\n  player_conditional_call(&D_801406C0[p].values[1]);\n  player_conditional_call(&D_801406C0[p].values[2]);\n}\n' % t + R(b1, '''  player_conditional_call(&D_801406C0[player].values[0]);
  player_conditional_call(&D_801406C0[player].values[1]);
  player_conditional_call(&D_801406C0[player].values[2]);
''', '  trio(player);\n')
