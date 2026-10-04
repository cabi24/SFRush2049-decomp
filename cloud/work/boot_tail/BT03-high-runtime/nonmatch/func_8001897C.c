/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct Context {
    u8 unknown000[0x118];
    u32 fraction118;
    s32 whole11C;
    s32 half120;
    u32 rate124;
    u8 unknown128[0xE9A];
    u16 scaleFC2;
} Context;
extern Context *D_8004BE80;
extern s32 D_8004F808;
extern s32 D_8004F800;
void func_8001897C(void)
{
    u32 value;
    value = ((D_8004BE80->rate124 * 3072U / 60U) *
        (u32)((s32)((u32)D_8004F808 << 16) / D_8004F800)) >> 3;
    value = (D_8004BE80->scaleFC2 * value) >> 8;
    D_8004BE80->fraction118 = value & 65535;
    D_8004BE80->whole11C = value >> 16;
    D_8004BE80->half120 = D_8004BE80->whole11C >> 1;
}
