/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef signed char s8;typedef signed short s16;typedef unsigned int u32;typedef float f32;
typedef struct Slot44 {u8 busy,enabled,unknown; s8 player;u8 primary[4],secondary[4],previous[4];s8 state;u8 gap[3];int mode;s16 first,second,third,fourth;u8 opaque[8];u32 deadline;} Slot44;
typedef struct Model772 {u8 prefix[30];s8 state;u8 tail[741];} Model772;
extern s8 D_80116DB4,D_80116DB8;
extern Slot44 D_80153FD8[][2];
extern Model772 D_80144018[];
extern f32 D_8002AFB4;
extern u8 D_8002E8E8[];
extern int func_800DDF28(int),func_800DDEA4(int);
extern void player_state_set(int,int),player_mode_set(int,int);
void display_settings(int player,int mode,int first,int second,int third,int fourth) {
 int i,j,slot;
 Slot44 *record,*row;
 if(mode==10)D_80116DB4=1;
 if(D_80116DB8==0) {
  for(i=0;i<4;i++)for(j=0;j<2;j++) {
   D_80153FD8[i][j].mode=0;
   D_80153FD8[i][j].busy=1;
  }
  D_80116DB8=1;
 }
 row=D_80153FD8[player];
 for(slot=0;slot<2;slot++)if(row[slot].mode==0)break;
 record=&row[slot];
 record->busy=0;
 record->deadline=(int)(3.0f*D_8002AFB4)+*(u32 *)(D_8002E8E8+636);
 record->enabled=1;
 record->unknown=0;
 record->mode=mode;
 for(i=0;i<4;i++) {
  record->primary[i]=func_800DDF28(i);
  record->secondary[i]=func_800DDEA4(i);
  record->previous[i]=0;
 }
 player_state_set(-1,0);
 player_mode_set(-1,0);
 if(mode==1) {
  player_state_set(-1,1);
  player_mode_set(-1,1);
 } else {
  record->player=player;
  if(mode==2) {
   player_state_set(-1,1);
   player_mode_set(-1,1);
  } else {
   player_state_set(record->player,1);
   player_mode_set(record->player,1);
  }
 }
 record->first=first;
 record->state=D_80144018[player].state;
 record->second=second;
 record->third=third;
 record->fourth=fourth;
}
