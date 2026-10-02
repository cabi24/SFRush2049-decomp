/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
extern f32 D_801543CC;extern s32 D_801161D0;extern s16 D_80151CEE;
void func_800EC270(void *vehicle,void *state) {
 FIELD(state,s16,824)=0;
 FIELD(state,f32,788)=0.0f;FIELD(state,f32,792)=0.0f;
 FIELD(state,s16,826)=-1;FIELD(state,f32,820)=D_801543CC;
 if(FIELD(vehicle,s8,1996)==1) {
  s32 index=D_801161D0;
  FIELD(state,s16,828)=index;FIELD(state,s16,830)=index;
  D_801161D0=++index;
  if(index>=4)D_801161D0=0;
 }else FIELD(state,s16,828)=0;
 if(D_80151CEE==0)FIELD(vehicle,s8,2012)=1;
 FIELD(state,s16,250)=-1;FIELD(state,s16,252)=0;FIELD(state,s16,254)=0;
}
