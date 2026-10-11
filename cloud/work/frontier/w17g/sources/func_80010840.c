/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80010840 (static, boot 0x80010840, 160 B, segment 0x113e0 -> src/rom/lib_113e0.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 14 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_runtime_gateway_80010840.
 * Body + declarations verbatim from cloud/work/boot_tail/BT02-final-small/nonmatch/func_80010840.c (Astra / BT02-final-small).
 * Shaping devices: none. */
extern unsigned char D_8002C630;
extern unsigned char D_8004FA18;
extern unsigned char D_8004FA19;
extern unsigned char D_8004FA1A;
extern int func_800143C0(unsigned int *, unsigned short, unsigned short, unsigned int);
extern int func_800107E0(unsigned int);

int func_80010840(unsigned int frequency, unsigned char voices,
                  unsigned char setting1, unsigned char setting2,
                  unsigned short count, unsigned int flags)
{
    int result;
    D_8002C630 = 0;
    if (voices <= 32) {
        D_8004FA18 = voices;
    } else {
        D_8004FA18 = 32;
    }
    D_8004FA19 = setting1;
    D_8004FA1A = setting2;
    result = func_800143C0(&frequency, D_8004FA18, count, flags);
    if (result == 0) {
        result = func_800107E0(frequency);
    }
    return result;
}
