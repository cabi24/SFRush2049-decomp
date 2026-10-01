/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
extern u32 D_80124EEC;
void func_800D0810(void *object) {
 s32 i,j;
 for(i=0;i<3;i++) {
 FIELD(object,f32,292+i*4)=0.0f;FIELD(object,f32,352+i*4)=0.0f;
 FIELD(object,f32,364+i*4)=0.0f;FIELD(object,f32,328+i*4)=0.0f;
 FIELD(object,f32,340+i*4)=0.0f;
 }
 for(i=0;i<4;i++) {
 FIELD(object,u32,928+i*4)=D_80124EEC;
 for(j=0;j<3;j++)FIELD(object,f32,196+i*12+j*4)=0.0f;
 }
}
