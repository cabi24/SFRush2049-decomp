typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define NULL ((void *)0)
s32 func_800BE744(u8 *input) {u8 *p=input+1;u32 byte; s32 count=0;if(input[0]==255){while(p[0] || p[1]){count++;p+=2;}}else if(input[0]){do{byte=*p++;count++;}while(byte);}return count;}
