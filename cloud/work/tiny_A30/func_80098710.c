/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
extern f32 D_80152748;
s32 func_80098710(void *object) {
 s32 original=FIELD(object,u8,27),value;f32 phase,end;
 value=original+(1.0f-FIELD(object,f32,36))*80.0f;
 if(FIELD(object,s8,25)!=0)return value+8;
 if(original!=0 && FIELD(object,f32,32)>=0.0f && FIELD(object,s32,16)==2) {
 phase=FIELD(object,f32,32);end=D_80152748;
 if(end<phase)end+=14400.0f;
 value=value+(end-phase)/0.25f;
 }
 return value;
}
