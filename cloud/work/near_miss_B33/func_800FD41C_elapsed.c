/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef unsigned int u32;
typedef float f32;
extern u8 D_8002E8E8[];
extern u32 D_80111958;
extern f32 D_8002AFB8;
f32 func_800FD41C(void) {
    return (f32)(*(u32*)(D_8002E8E8+0x27C)-D_80111958)*D_8002AFB8;
}
