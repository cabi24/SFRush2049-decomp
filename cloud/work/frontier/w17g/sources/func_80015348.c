/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80015348 (static, boot 0x80015348, 36 B, segment 0x15a10 -> src/rom/lib_15a10.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 1 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_pool_state_80015348.
 * Body + declarations verbatim from cloud/work/boot_tail/BT03-low-B/func_80015348.c (Astra / BT03-low-B).
 * Shaping devices: none. */
extern int func_8001661C(unsigned short);

void func_80015348(unsigned short id)
{
    func_8001661C(id);
}
