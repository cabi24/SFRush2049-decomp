/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001C19C (static, boot 0x8001C19C, 60 B, segment 0x1a660 -> src/rom/lib_1a660.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 11 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_command_prepare_8001C19C.
 * Body + declarations verbatim from cloud/work/boot_tail/BT03-high/nonmatch/func_8001C19C.c (Astra / BT03-high).
 * Shaping devices: none. */
extern unsigned char D_8002C630;
extern unsigned char D_8004F314[][40];
void func_8001C19C(unsigned char channel, unsigned char value)
{ if (D_8002C630) D_8004F314[channel][0] = value; }
