exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/v8.py').read().split('V = {}')[0])
def A(s): return R(s, ' s8 *ready;\n', ' f32 vec[3];\n s8 *ready;\n')
V = {}
V['st'] = H1 + HS + A(R(b1, '&D_80140420[player]', 'pst(player)'))
V['def'] = H1 + HD + A(R(b1, '&D_8010FD80[vehicle->category]', 'pdef(vehicle->category)'))
V['ex'] = H1 + HE + A(R(b1, '&D_80140640[player]', 'pex(player)'))
V['vals'] = H1 + HV + A(R(b1, '''  player_conditional_call(&D_801406C0[player].values[0]);
  player_conditional_call(&D_801406C0[player].values[1]);
  player_conditional_call(&D_801406C0[player].values[2]);''', '''  player_conditional_call(&pvals(player)->values[0]);
  player_conditional_call(&pvals(player)->values[1]);
  player_conditional_call(&pvals(player)->values[2]);'''))
