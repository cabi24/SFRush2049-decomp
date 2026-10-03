/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Controller-table access; see cloud/work/boot_tail/C13-medium/README.md. */
extern unsigned char D_80050D00[][16][134];
extern unsigned char D_80055000[][134];

void func_80020610(unsigned char controller, unsigned char channel,
                   unsigned char set, unsigned char value)
{
    if (set != 255) {
        D_80050D00[set][channel][controller] = value & 127;
    } else {
        D_80055000[channel][controller] = value & 127;
    }
}
