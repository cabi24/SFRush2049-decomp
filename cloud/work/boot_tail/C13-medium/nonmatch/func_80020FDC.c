/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Controller-table access; see cloud/work/boot_tail/C13-medium/README.md. */
extern unsigned char D_800560C0[][16];
extern unsigned char D_80056140[];

void func_80020FDC(unsigned char channel, unsigned char set,
                   unsigned char value)
{
    if (set != 255) {
        D_800560C0[set][channel] = value;
    } else {
        D_80056140[channel] = value;
    }
}
