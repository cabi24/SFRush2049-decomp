HEAD = r'''void func_800D5E64(Vehicle2056 *vehicle)
{
 s16 player=vehicle->player;
 s8 *ready;
 Player84 *state;
 State20 *pair;
 int i;
 s32 object;
 if(!D_8010FFC0 || vehicle->disabled)return;
 ready=&D_8010FFC4[player];
 state=&D_80140420[player];
 state->definition=&D_8010FD80[vehicle->category];
'''
TAIL = r''' player_conditional_call(&state->extra);
 player_conditional_call(&D_80140640[player]);
 if(player<4) {
  player_conditional_call(&D_801406C0[player].values[0]);
  player_conditional_call(&D_801406C0[player].values[1]);
  player_conditional_call(&D_801406C0[player].values[2]);
  D_80140AE0[player]=-1;
  best_times_display(player);
  D_801407E0[player]=-1;
  D_801407C0[player]=-1;
 }
 *ready=1;
}
'''
L_idx = r''' for(i=0;i<2;i++) {
  player_conditional_call(&state->pair[i]);
  object=state->definition->item[i].object;
  if(object!=-1) {
   if(vehicle->mode==2)state->pair[i].handle=high_scores_display(object,player,0,0,0.0f,0.0f,0.0f);
   else state->pair[i].handle=camera_target_track(D_801141B0,D_801141B0,400.0f,0.0f,1.0f,0.0f,object,player,0,128);
  }
 }
'''
L_mix = r''' for(i=0,pair=state->pair;i<2;i++,pair++) {
  player_conditional_call(pair);
  object=state->definition->item[i].object;
  if(object!=-1) {
   if(vehicle->mode==2)state->pair[i].handle=high_scores_display(object,player,0,0,0.0f,0.0f,0.0f);
   else state->pair[i].handle=camera_target_track(D_801141B0,D_801141B0,400.0f,0.0f,1.0f,0.0f,object,player,0,128);
  }
 }
'''
V = {
 'idx': HEAD + L_idx + TAIL,
 'mix': HEAD + L_mix + TAIL,
 'idxu': HEAD.replace('s32 object','u32 object') + L_idx + TAIL,
 'mixu': HEAD.replace('s32 object','u32 object') + L_mix + TAIL,
}
