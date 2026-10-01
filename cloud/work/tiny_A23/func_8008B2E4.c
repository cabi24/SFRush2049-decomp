typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
extern u32 D_8011735C;
f32 func_8008B2E4(f32 range) {u32 seed;seed=D_8011735C*0x41C64E6D+12345;D_8011735C=seed;return (f32)(((s32)seed>>16)&0x7FFF)*range/32768.0f;}
