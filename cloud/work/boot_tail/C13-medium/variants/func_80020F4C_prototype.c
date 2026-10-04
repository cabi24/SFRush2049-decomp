/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Controller-table access; see cloud/work/boot_tail/C13-medium/README.md. */
extern unsigned char D_80050C60[][16];
extern unsigned char D_80050CE0[];

void func_80020F4C(unsigned char channel, unsigned char set,
                   unsigned char value);

void func_80020F4C(unsigned char channel, unsigned char set,
                   unsigned char value)
{
    if (set != 255) {
        D_80050C60[set][channel] = value;
    } else {
        D_80050CE0[channel] = value;
    }
}
