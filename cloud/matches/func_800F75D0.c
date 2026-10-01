/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
extern s8 D_80150F00[][5];
s8 func_800F75D0(s32 row,s32 column) {return D_80150F00[row][column];}
