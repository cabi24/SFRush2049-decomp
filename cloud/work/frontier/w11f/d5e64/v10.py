exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/v1.py').read().split('V = {')[0])
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
B = R(HEAD + L_idx + TAIL, 's16 player', 's32 player')
H1 = 'static s8 *rdy(s32 p) { return &D_8010FFC4[p]; }\n'
b1 = R(B, '&D_8010FFC4[player]', 'rdy(player)')
V = {}
V['trio'] = H1 + 'static void trio(s32 p) {\n  player_conditional_call(&D_801406C0[p].values[0]);\n  player_conditional_call(&D_801406C0[p].values[1]);\n  player_conditional_call(&D_801406C0[p].values[2]);\n}\n' + R(b1, '''  player_conditional_call(&D_801406C0[player].values[0]);
  player_conditional_call(&D_801406C0[player].values[1]);
  player_conditional_call(&D_801406C0[player].values[2]);
''', '  trio(player);\n')
V['tail'] = H1 + '''static void tailf(s32 p) {
  player_conditional_call(&D_801406C0[p].values[0]);
  player_conditional_call(&D_801406C0[p].values[1]);
  player_conditional_call(&D_801406C0[p].values[2]);
  D_80140AE0[p]=-1;
  best_times_display(p);
  D_801407E0[p]=-1;
  D_801407C0[p]=-1;
}
''' + R(b1, '''  player_conditional_call(&D_801406C0[player].values[0]);
  player_conditional_call(&D_801406C0[player].values[1]);
  player_conditional_call(&D_801406C0[player].values[2]);
  D_80140AE0[player]=-1;
  best_times_display(player);
  D_801407E0[player]=-1;
  D_801407C0[player]=-1;
''', '  tailf(player);\n')
V['snd'] = H1 + '''static s32 sstart(s32 object, s32 player, s8 mode) {
  if(mode==2)return high_scores_display(object,player,0,0,0.0f,0.0f,0.0f);
  return camera_target_track(D_801141B0,D_801141B0,400.0f,0.0f,1.0f,0.0f,object,player,0,128);
}
''' + R(b1, '''   if(vehicle->mode==2)state->pair[i].handle=high_scores_display(object,player,0,0,0.0f,0.0f,0.0f);
   else state->pair[i].handle=camera_target_track(D_801141B0,D_801141B0,400.0f,0.0f,1.0f,0.0f,object,player,0,128);
''', '   state->pair[i].handle=sstart(object,player,vehicle->mode);\n')
