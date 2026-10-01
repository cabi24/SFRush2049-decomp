/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
extern s8 D_80152570,D_8014978C;extern s16 D_801613D0[],D_80117408[],D_80154348;extern s16 *D_801173D8[];
void func_800B9284(void) {
 s32 index,next;
 if(D_80152570)index=D_8014978C+6;else index=D_8014978C;
 next=D_801613D0[index]+1;
 if(next<0||next>=D_80117408[index])next=0;
 D_801613D0[index]=next;
 D_80154348=D_801173D8[index][next];
}
