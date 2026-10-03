/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct Car {u8 bytes[772];} Car;
typedef struct Input {u8 pad0[2];u8 flags;u8 pad3;} Input;
extern Car D_80144030[4];extern Input D_80149440[4];extern s8 D_8011EAE0;
void func_800A43FC(void) {
 Car *car;Input *input;
 if(D_8011EAE0) {
 input=D_80149440;
 for(car=D_80144030;car!=D_80144030+4;car++,input++) {
 if((input->flags&1)&&!(input->flags&2)) {if(FIELD(car,s8,2)==0)FIELD(car,s8,2)=1;}
 else if(FIELD(car,s8,2)!=0) {FIELD(car,s8,2)=0;FIELD(car,s8,3)=1;}
 }
 }
}
