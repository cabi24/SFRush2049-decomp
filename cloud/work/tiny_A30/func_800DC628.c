/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
extern s32 D_801170F8;extern u8 *D_80116FE4;extern u8 D_80116FE8[],D_8012E618[];extern u16 D_801170E8,D_801170EC,D_801170F0,D_801170F4;
s32 func_800DC628(u32 first,u32 count) {
 s32 i;u16 bytes;u8 *p;
 if(D_801170F8) {D_801170F8=0;for(i=0;i<32;i++)D_80116FE8[D_80116FE4[i]]=i;}
 bytes=(first+count+7)>>3;D_801170E8=bytes;
 if(bytes>32)return 0;if(count>32)return 0;
 for(p=D_8012E618;p<D_8012E618+bytes;p++)*p=0;
 D_801170EC=first;D_801170F0=count;D_801170F4=0;
 return 1;
}
