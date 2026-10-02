/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned short u16;
typedef unsigned int u32;
extern u32 D_80124FC8;
void func_800A5560(u16 color) {
    D_80124FC8 = (color << 16) | color;
}
