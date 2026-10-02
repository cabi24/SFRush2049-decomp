/* flags: -g0 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef unsigned int u32;
typedef float f32;
extern u8 D_8002E8E8[];
extern u32 D_80111958;
extern f32 D_8002AFB8;
f32 func_800FD41C(void) {
    u32 ticks=*(u32*)(D_8002E8E8+0x27C)-D_80111958;
    f32 scale=D_8002AFB8;
    return (f32)ticks*scale;
}
