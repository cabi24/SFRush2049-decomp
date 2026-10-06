typedef unsigned short u16; typedef unsigned int u32; typedef int s32;
extern u32 D_80124FC8;
void func_800A5560(s32 color) { D_80124FC8 = ((color & 0xFFFF) << 16) | (color & 0xFFFF); }
