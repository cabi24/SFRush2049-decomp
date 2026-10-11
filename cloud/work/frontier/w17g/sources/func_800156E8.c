/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800156E8 (static, boot 0x800156E8, 56 B, segment 0x15a10 -> src/rom/lib_15a10.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 8 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_pool_state_800156E8.
 * Body + declarations verbatim from cloud/work/boot_tail/BT03-low-C/func_800156E8.c (Astra / BT03-low-C).
 * Shaping devices: none. */
extern int func_8001558C(unsigned short, unsigned short, unsigned int, unsigned int, unsigned char);

int func_800156E8(unsigned short group, unsigned short item, unsigned int context, unsigned int option)
{
    return func_8001558C(group, item, context, option, 0);
}
