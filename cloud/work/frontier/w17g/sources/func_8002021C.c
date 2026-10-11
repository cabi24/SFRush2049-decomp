/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8002021C (static, boot 0x8002021C, 88 B, segment 0x207b0 -> src/rom/lib_207b0.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 12 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_parameter_8002021C.
 * Body + declarations verbatim from cloud/work/boot_tail/BT03-high-params/nonmatch/func_8002021C.c (Astra / BT03-high-params).
 * Shaping devices: none. */
typedef unsigned char u8;
typedef signed short s16;
extern s16 D_8004BE98[16];
extern void func_80014594(void);
extern void func_800145DC(void);
s16 func_8002021C(u8 channel, s16 value)
{
    s16 previous;
    channel &= 15;
    func_80014594();
    previous = D_8004BE98[channel];
    D_8004BE98[channel] = value;
    func_800145DC();
    return previous;
}
