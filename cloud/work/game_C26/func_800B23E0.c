/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
typedef int s32;
typedef unsigned int u32;
typedef unsigned char u8;
extern s32 D_8011735C;
extern u32 D_80123418[];
u8 func_800B23E0(u8 index) {
    u32 mask = D_80123418[index];
    u8 choice;
    do {
        D_8011735C = D_8011735C * 1103515245U + 12345;
        choice = (u32)((f32)((D_8011735C >> 16) & 0x7fff) * 32.0f / 32768.0f);
    } while (!(mask & (1 << choice)));
    return choice;
}
