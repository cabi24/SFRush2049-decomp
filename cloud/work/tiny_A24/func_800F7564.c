typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define NULL ((void *)0)
extern s16 D_8014A108;
extern s8 D_80150E88[][4];
s32 func_800F7564(s32 field) {s32 found=0;s8 *p=(s8 *)D_80150E88; s8 *end=p+D_8014A108*4;if(D_8014A108>0){do{if(p[field])found=1;p+=4;}while(p<end);}return found;}
