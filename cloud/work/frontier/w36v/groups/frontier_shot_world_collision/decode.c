/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Expand a big-endian run word; matching values fill from the front,
 * others from the back. No direct arcade equivalent is established. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void func_800ADCE0(u8 *input, s32 count, u16 *front, u32 marker)
{
    u16 *back = front + count - 1;
    u32 value;
    s32 run;

    while (count > 0) {
        value = ((u32)input[0] << 8) + input[1];
        input += 2;
        run = value >> 13;
        value &= 0x1FFF;
        count = count - run - 1;
        do {
            run--;
            if (value == marker) {
                *front++ = value++;
            } else {
                *back-- = value++;
            }
        } while (run >= 0);
    }
}
