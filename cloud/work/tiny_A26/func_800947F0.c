/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
extern f32 D_80118E30,D_8002EB94,D_8017A630;
void func_800947F0(void) {
 D_80118E30+=D_8002EB94;
 D_8017A630=D_80118E30+D_80118E30;
 D_8017A630-=(s32)D_8017A630;
 if(D_8017A630>0.5f)D_8017A630=1.0f-D_8017A630;
 D_8017A630=D_8017A630+D_8017A630;
}
