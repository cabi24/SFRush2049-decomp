/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001C77C (static, boot 0x8001C77C, 120 B, segment 0x1cf90 -> src/rom/lib_1cf90.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 19 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_sequence_state_8001C77C.
 * Body + declarations verbatim from cloud/work/boot_tail/BT03-high-service/nonmatch/func_8001C77C.c (Astra / BT03-high-service).
 * Shaping devices: none. */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_80014A74(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void func_8001C77C(unsigned int channel, unsigned char first, unsigned char second, unsigned char third, unsigned char fourth)
{
    if (D_8002C630 && channel != 0xFFFFFFFF) {
        func_80014594();
        func_80014A74(channel, first << 16, second << 16, third << 16, fourth << 16);
        func_800145DC();
    }
}
