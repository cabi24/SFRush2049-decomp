/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
extern u8 D_80146204,D_8017A63C;
s32 func_800A73FC(s32 value) {
 if(value<=0)return D_80146204;
 D_80146204=value;D_8017A63C=0;return value;
}
