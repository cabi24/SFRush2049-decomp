/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native table reset; original table names and middleware release are unknown. */
typedef unsigned char u8;
typedef unsigned int u32;
extern u8 D_80056160[8][16];
extern u8 D_800561E0[32];
void func_800218CC(void)
{
    u32 row;
    u32 index;
    for (row = 0; row < 8; row++) {
        for (index = 0; index < 16; index++) {
            D_80056160[row][index] = 255;
        }
    }
    for (index = 0; index < 32; index++) {
        D_800561E0[index] = 255;
    }
}
