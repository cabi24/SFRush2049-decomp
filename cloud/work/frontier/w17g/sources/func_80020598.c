/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80020598 (static, boot 0x80020598, 76 B, segment 0x207b0 -> src/rom/lib_207b0.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 16 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_initialization_80020598.
 * Body + declarations verbatim from cloud/work/boot_tail/BT03-high-init/nonmatch/func_80020598.c (Astra / BT03-high-init).
 * Shaping devices: none. */
typedef struct CallbackWords { unsigned int words[8]; } CallbackWords;
extern CallbackWords D_80038000;

void func_80020598(const CallbackWords *callbacks)
{
    D_80038000 = *callbacks;
}
