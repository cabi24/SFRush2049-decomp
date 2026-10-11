/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001989C (static, boot 0x8001989C, 44 B, segment 0x17dc0 -> src/rom/lib_17dc0.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 2 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_channel_control_8001989C.
 * Body + declarations verbatim from cloud/work/boot_tail/BT03-high/nonmatch/func_8001989C.c (Astra / BT03-high).
 * Shaping devices: none. */
unsigned char func_8001989C(unsigned int *state) { if (*state != 0xFFFFFFFF) return (*state & 0x80000000) == 0; return 1; }
