typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct {s16 pad0,first;u8 pad4[4];s16 count;} Path;
extern Path D_80151CE8;
s32 func_800B930C(s32 index) {if(index+1 == D_80151CE8.count)return D_80151CE8.first;return index+1;}
