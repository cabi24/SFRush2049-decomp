/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct Car {u8 pad0[1808];s32 ticks;f32 value,rate;u8 pad1820[236];} Car;
extern s32 D_80143FF4,D_8014A110;extern f32 D_8002AFB8,D_801543CC;extern Car D_8014A250[6];
void func_800E762C(u32 ticks) {
 s32 negative=(s32)(0u-ticks);f32 value,ratio;Car *car;
 D_80143FF4=negative;value=negative*D_8002AFB8;D_801543CC=value;
 for(car=D_8014A250;car!=D_8014A250+6;car++) {
 if(D_8014A110!=2||car==D_8014A250||car->rate==0.0f) {car->ticks=negative;car->value=value;car->rate=D_8002AFB8;}
 else {ratio=value/car->rate;if(ratio<0.0f)car->ticks=(s32)(ratio-0.5f);else car->ticks=(s32)(ratio+0.5f);car->value=value;}
 }
}
