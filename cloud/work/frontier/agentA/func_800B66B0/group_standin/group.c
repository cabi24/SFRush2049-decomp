/* flags: -g0 -O3 -mips2 -G 0 -non_shared (IPA group member) */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

s32 func_800B66B0(u8 *str, s16 max) {
    s32 count;
    u32 len;
    u8 c;

    count = 0;
    if (*str == 0xFF) {
        len = 1;
        do {
            len += 2;
            count++; if (str[len - 1] == 0 && str[len - 2] == 0) {
            break;
            }
        } while (max < 0 || count <= max);
    } else {
        len = 0;
        do {
            c = *str;
            len++;
            count++;
            str++;
            if (c == 0) {
                break;
            }
        } while (max < 0 || count <= max);
    }
    return len;
}

extern u8 D_zz[];
s32 zz_a(void) { return func_800B66B0(D_zz, 3); }
s32 zz_b(u8 *p, s16 m) { return func_800B66B0(p, m) + func_800B66B0(p + 1, m); }
