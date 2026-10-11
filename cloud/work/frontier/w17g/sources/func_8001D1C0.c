/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001D1C0 (static, boot 0x8001D1C0, 52 B, segment 0x1cf90 -> src/rom/lib_1cf90.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 3 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_sequence_state_8001D1C0.
 * Body + declarations verbatim from cloud/work/boot_tail/BT03-high-next/nonmatch/func_8001D1C0.c (Astra / BT03-high-next).
 * Shaping devices: none. */
extern unsigned char D_8002C630;
unsigned char func_8001D1C0(unsigned int *state)
{
    if (D_8002C630) return (state[2] & 0x10000) != 0;
    return 0;
}
