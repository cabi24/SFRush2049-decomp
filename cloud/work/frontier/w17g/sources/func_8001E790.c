/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001E790 (static, boot 0x8001E790, 44 B, segment 0x1f340 -> src/rom/lib_1f340.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 2 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): MusyX sndRand source lead.
 * Body + declarations verbatim from cloud/work/boot_tail/C11-small/func_8001E790_NONMATCH.c (Astra / C11-small).
 * Shaping devices: none. */
extern unsigned int D_8002CC50;

unsigned short func_8001E790(void)
{
    D_8002CC50 *= 2822053219U;
    return D_8002CC50 >> 6;
}
