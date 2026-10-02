/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef int s32;
void Input_ApplyPadConfig(void *);
typedef struct Player76 {u8 p0,car;u8 pad2[74];} Player76;
typedef struct Car772 {u8 pad0[6];s8 flag;u8 pad7[765];} Car772;
extern s16 D_8014A108;extern Player76 D_8014A118[];extern Car772 D_80144030[];
s32 audio_channel_reset(void *config) {
 Player76 *player,*end;s32 all=1;
 s16 count=D_8014A108;
 if(count>0) {
  player=D_8014A118;end=player+count;
  do {
   if(D_80144030[player->car].flag==0)all=0;
   player++;
  }while(player<end);
 }
 if(all!=*(s8*)((u8*)config+26)) {
  *(s8*)((u8*)config+26)=all;
  Input_ApplyPadConfig(config);
 }
 return 1;
}
