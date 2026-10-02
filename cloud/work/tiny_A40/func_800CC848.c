/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct Car {u8 bytes[772];} Car;extern Car D_80144030[];
s32 func_800A1A60(void *);
s32 func_800CC848(void **object,s32 action) {
 void *node=FIELD(*object,void *,8);s32 car;
 if(!node)return 1;
 car=FIELD(FIELD(node,void *,0),u8,16);
 if(FIELD(&D_80144030[car],s8,1)==0)return 0;
 if(!action)return 1;
 return func_800A1A60(node);
}
