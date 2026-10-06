static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }
void func_800D5E64(Vehicle2056 *vehicle)
{
 s32 player=vehicle->player;
 f32 vec[3];
 s8 *ready;
 Player84 *state;
 int i;
 s32 object;
 if(!D_8010FFC0 || vehicle->disabled)return;
 {
 s8 *r = rdy(player);
 state=&D_80140420[player];
 state->definition=&D_8010FD80[vehicle->category];
 ready = r;
 }
 for(i=0;i<2;i++) {
  player_conditional_call(&state->pair[i]);
  object=state->definition->item[i].object;
  if(object==-1) continue;
  {
   if(vehicle->mode==2)state->pair[i].handle=high_scores_display(object,player,0,0,0.0f,0.0f,0.0f);
   else state->pair[i].handle=camera_target_track(D_801141B0,D_801141B0,400.0f,0.0f,1.0f,0.0f,object,player,0,128);
  }
 }
 player_conditional_call(&state->extra);
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
