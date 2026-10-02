/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
extern u32 D_801174B4,D_8014A110;extern s16 D_8014A108;
extern f32 D_80149A78[][8],D_80144DA8[];extern u8 D_80144018[];
void func_800D2054(s32 index,f32 value) {
 f32 *row,*slot;s32 i,count;u8 *position;
 if((D_801174B4&8)||D_8014A110==1||index>=D_8014A108)return;
 row=D_80149A78[index];position=&D_80144018[index];count=*position;slot=&row[count];
 *slot=value;
 for(i=0;i<count;i++)*slot-=row[i];
 if(*slot<D_80144DA8[index])D_80144DA8[index]=*slot;
 *position=count+1;
}
