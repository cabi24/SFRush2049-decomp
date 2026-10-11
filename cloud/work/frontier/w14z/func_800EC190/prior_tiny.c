/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct Entry {s8 mode;u8 pad1[4];u8 ordinal,flag,value;} Entry;
extern s8 D_8014978C,D_8014A118,D_80150C04,D_80150F14;extern s16 D_801543CA,D_801163F4;extern Entry D_80153E88[];
void func_800EC190(s8 mode) {
 s16 i,j,index;
 D_8014978C=mode;D_801543CA=6;i=0;j=0;
 do {
 if(i>=6) {D_80153E88[i].flag=0;D_80153E88[i].value=i+6;}
 else {D_80153E88[i].ordinal=j++;D_80153E88[i].value=0;D_80153E88[i].flag=176;D_80153E88[i].mode=D_8014978C;}
 i++;
 }while(i<6);
 index=D_801163F4+1;if(index>=6)index=0;
 D_8014A118=index;D_80150C04=index;D_80150F14=2;D_801163F4=index;
}
