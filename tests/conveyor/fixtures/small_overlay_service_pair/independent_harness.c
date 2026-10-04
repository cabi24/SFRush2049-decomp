/* Independent host-only boundary doubles; no native or matching claims. */
#include "../../../../cloud/work/small_overlay_service_pair/service_pair.h"
SmallPlayer small_players[12];
SmallModel small_models[128];
SmallS8 small_teams[128];
SmallS16 small_player_count;
int callback_count, callback_source, callback_target, callback_code, rewrite_index = -1;
void func_800C55E4(int a,int b,int c) {
 callback_count++; callback_source=a; callback_target=b; callback_code=c;
 if(rewrite_index>=0)small_players[1].car_index=rewrite_index;
}
float func_8008C768(float x,float z) {(void)x;(void)z;return 0.0f;}
void func_800A61B0(float *a,float *b,void *m) {(void)m;b[0]=a[0];b[1]=a[1];b[2]=a[2];}
int run_case(int rem,int amount,int flag,int model,int source_id,int target_id,int same_team,int rewrite) {
 int i;
 for(i=0;i<128;i++){small_models[i].excluded=0;small_teams[i]=(SmallS8)i;}
 small_players[0].car_index=(SmallS8)source_id;
 small_players[1].car_index=(SmallS8)target_id;
 small_players[1].remaining=(SmallS16)rem;
 small_players[1].last_amount=12345;
 small_players[1].flags=(unsigned int)flag;
 small_models[target_id].excluded=(SmallS8)model;
 if(same_team)small_teams[source_id]=small_teams[target_id];
 callback_count=0;rewrite_index=rewrite;
 small_8038D3A4(&small_players[0],&small_players[1],amount);
 return (int)small_players[1].remaining;
}
int amount_result(void){return small_players[1].last_amount;}
int model_result(int i){return small_models[i].excluded;}
