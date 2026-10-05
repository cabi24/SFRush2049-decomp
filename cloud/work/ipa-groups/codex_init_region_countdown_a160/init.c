/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned int u32;
typedef struct Config {u8 type,other1[4],selection,flags,kind;} Config;
typedef struct Player {u8 other0,id,other2[74];} Player;
extern s16 D_8014A108,D_80142724,D_801543CA;
extern int D_8014A110;extern s8 D_80154628,D_8014978C;
extern u8 D_801543D4;extern Player D_80154450[];
extern Config D_80153E88[];extern u32 D_8011735C;
void init_state_continue(void)
{
 int extra,slot,selected,prior;u8 next=0;Config *config;
 extra=6-D_8014A108;
 if(D_80142724<extra)extra=D_80142724;
 if(D_8014A110==4 || D_8014A110==5 || D_8014A110==6)extra=0;
 D_801543CA=D_8014A108+extra;
 config=D_80153E88;
 for(slot=0;slot<6;slot++,config++) {
  if(D_8014A110==3 && slot<D_801543CA && D_80154628>0) {
   if(slot==0)selected=D_801543D4;
   else if(slot==1)selected=D_801543D4^1;
   else selected=slot;
   config->selection=D_80154450[selected].id;
   config->flags=176;
   if(slot<D_8014A108)config->kind=6;
   else config->kind=0;
  } else if(slot<D_8014A108) {
   config->selection=extra++;
   config->kind=6;config->flags=176;
  } else if(slot<D_801543CA) {
   if(D_8014A110==3) {
    do {
     D_8011735C=D_8011735C*0x41C64E6D+12345;
     config->selection=(u32)((float)((D_8011735C>>16)&32767)*(float)D_801543CA/32768.0f);
     for(prior=0;prior<slot;prior++) {
      if(config->selection==D_80153E88[prior].selection)break;
     }
    } while(prior!=slot);
    config->kind=0;
   } else {
    config->selection=next++;
    config->kind=0;
   }
   config->flags=176;
  } else {
   config->kind=7;config->flags=0;
  }
  config->type=D_8014978C;
 }
}
