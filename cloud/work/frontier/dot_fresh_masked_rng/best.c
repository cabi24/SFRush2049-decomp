/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*
 * Unaccepted reconstruction: choose a random bit allowed by table[index].
 * The signed seed view preserves the native arithmetic right shift; the
 * unsigned multiplier gives the LCG its modulo-2^32 update. The explicit
 * float value preserves the observed unsigned float-conversion sequence.
 * A zero mask never terminates, as in the native body. Table storage and
 * valid indices remain owned by the game; no table contents are reproduced.
 * Strict result: 13/67 relocated words differ, exact 268-byte ELF extent.
 */
typedef float f32;
typedef int s32;
typedef unsigned int u32;
typedef unsigned char u8;
extern s32 D_8011735C;
extern u32 D_80123418[];
u8 func_800B23E0(u8 index) {
    u32 mask = D_80123418[index];
    u8 choice;
    f32 value;
    do {
        D_8011735C = D_8011735C * 1103515245U + 12345;
        value = (f32)((D_8011735C >> 16) & 0x7fff) * 32.0f / 32768.0f;
        choice = (u32)value;
    } while (!(mask & (1U << choice)));
    return choice;
}
