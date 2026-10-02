/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct Player {u8 pad0[25];u8 path;u8 pad26[50];} Player;
extern Player D_8014A100[];extern s16 D_8014A0F8[];extern s8 D_8013C068[][10];extern s8 D_80114060[];
s32 func_800D8078(s8 player) {
 s32 index=D_8014A0F8[player];s8 *path=D_8013C068[D_8014A100[player].path];s32 current=path[index],result;
 result=current!=23;
 if(result) {
 result=D_80114060[current]==1;
 if(result) {
  if(index==2) {
   current=path[2];result=current==25 || current==19;
  }
  if(result && index>=3) {result=path[index]!=19;if(result)result=path[index]!=25;}
 }
 }
 return result;
}
