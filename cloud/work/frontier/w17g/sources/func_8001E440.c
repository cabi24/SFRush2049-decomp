/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001E440 (static, boot 0x8001E440, 204 B, segment 0x1ece0 -> src/rom/lib_1ece0.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 23 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): numeric_memory_utility_8001E440.
 * Body + declarations verbatim from cloud/work/boot_tail/C11-numeric/func_8001E440_NONMATCH.c (Astra / C11-numeric).
 * Shaping devices: none. */
typedef unsigned short u16;
typedef unsigned int u32;
extern const float D_8002D924;
u16 func_8001E440(u16 value)
{
    return (u16)(u32)((float)value * D_8002D924);
}
