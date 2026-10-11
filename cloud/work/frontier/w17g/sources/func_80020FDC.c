/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80020FDC (static, boot 0x80020FDC, 76 B, segment 0x21210 -> src/rom/lib_21210.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 18 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_data_dispatch_80020FDC.
 * Body + declarations verbatim from cloud/work/boot_tail/C13-medium/nonmatch/func_80020FDC.c (Astra / C13-medium).
 * Shaping devices: none. */
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
