/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* __osContAddressCrc (static, boot 0x8000F9F0, 208 B, segment 0x105f0 -> src/rom/lib_105f0.c, unconverted)
 * Origin: reference/repos/ultralib (decompals e24c836) src/io/crc.c, BUILD_VERSION >= VERSION_J body
 *         (shift-register CRC-5, generator 0x15). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

u8 __osContAddressCrc(u16 addr) {
    u32 temp = 0;
    u32 i = (1 << 10);

    do {
        temp <<= 1;

        if ((u32)addr & i) {
            if (temp & (1 << 5)) {
                temp ^= 0x15 - 1;
            } else {
                ++temp;
            }
        } else if (temp & (1 << 5)) {
            temp ^= 0x15;
        }

        i >>= 1;
    } while (i != 0);

    i = 5;

    do {
        temp <<= 1;
        if (temp & (1 << 5)) {
            temp ^= 0x15;
        }
    } while (--i != 0);

    return temp & ((1 << 5) - 1);
}
