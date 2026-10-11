/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80020610 (static, boot 0x80020610, 156 B, segment 0x21210 -> src/rom/lib_21210.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 39 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_data_dispatch_80020610.
 * Body + declarations verbatim from cloud/work/boot_tail/C13-medium/nonmatch/func_80020610.c (Astra / C13-medium).
 * Shaping devices: none. */
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
