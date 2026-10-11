/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* __osContDataCrc (static, boot 0x8000FAC0, 160 B, segment 0x105f0 -> src/rom/lib_105f0.c, unconverted)
 * Conveyor inventory target_id: __osPfsDataChecksum (stale pre-S12 label; same address and object).
 * (The inventory row named __osContDataCrc at 0x8000FA4C is a 128 B fragment of __osContAddressCrc.)
 * Origin: reference/repos/ultralib (decompals e24c836) src/io/crc.c, BUILD_VERSION >= VERSION_J body
 *         (shift-register CRC-8, generator 0x85). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

u8 __osContDataCrc(u8* data) {
    u32 temp = 0;
    u32 i;
    u32 j;

    for (i = 32; i; --i) {
        for (j = (1 << (8 - 1)); j; j >>= 1) {
            temp <<= 1;
            if ((*data & j) != 0) {
                if ((temp & (1 << 8)) != 0) {
                    temp ^= 0x85 - 1;
                } else {
                    ++temp;
                }
            } else if (temp & (1 << 8)) {
                temp ^= 0x85;
            }
        }

        data++;
    }
    do {
        temp <<= 1;
        if (temp & (1 << 8)) {
            temp ^= 0x85;
        }
    } while (++i < 8);

    return temp;
}
