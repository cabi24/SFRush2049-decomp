/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
extern s8 D_80111954,D_80111968;extern volatile u8 D_80111950;extern u8 D_80149AF8[8];
extern u8 D_801497A8[],D_80152730[];
void player_state_set(s32,s32);void player_mode_set(s32,s32);void osCreateMesgQueue(void *,void *,s32);void osJamMesg(void *,void *,s32);void controller_poll(void);
void func_800E7038(void) {
 if(!D_80111954) {player_state_set(-1,1);player_mode_set(-1,1);}
 D_80111954=1;
 D_80149AF8[6]=D_80149AF8[7]=70;
 D_80149AF8[4]=D_80149AF8[5]=70;
 D_80149AF8[2]=D_80149AF8[3]=70;
 D_80149AF8[0]=D_80149AF8[1]=70;
 if(!D_80111968) {D_80111968=1;osCreateMesgQueue(D_801497A8,D_80152730,1);osJamMesg(D_801497A8,0,0);}
 if(D_80111950==128)while(D_80111950==128) {}
 controller_poll();
}
