/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
u32 func_800B0EA0(s32 ratio,u32 left,u32 right) {
 s32 inverse;u32 red,green,blue;
 if(ratio==0)return left;
 if(ratio>=255)return right;
 inverse=255-ratio;
 red=((s32)((left&0xF800)*(u32)inverse+(right&0xF800)*(u32)ratio)>>8)&0xF800;
 green=((s32)((left&0x7C0)*(u32)inverse+(right&0x7C0)*(u32)ratio)>>8)&0x7C0;
 blue=((s32)((left&0x3E)*(u32)inverse+(right&0x3E)*(u32)ratio)>>8)&0x3E;
 return red|green|blue|1;
}
