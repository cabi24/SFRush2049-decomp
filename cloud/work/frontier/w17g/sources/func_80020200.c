/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80020200 (static, boot 0x80020200, 28 B, segment 0x207b0 -> src/rom/lib_207b0.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 3 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_sequence_api_80020200.
 * Body + declarations verbatim from cloud/work/boot_tail/BT03-high-next/nonmatch/func_80020200.c (Astra / BT03-high-next).
 * Shaping devices: none. */
extern short D_8004BE98[];
short func_80020200(unsigned char value)
{
    return D_8004BE98[value & 15];
}
