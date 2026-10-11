/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80018BEC (static, boot 0x80018BEC, 64 B, segment 0x17dc0 -> src/rom/lib_17dc0.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 3 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_channel_control_80018BEC.
 * Body + declarations verbatim from cloud/work/boot_tail/BT03-high-control/nonmatch/func_80018BEC.c (Astra / BT03-high-control).
 * w17g edit: removed the D10 file's unused `VoiceState` typedef and `extern VoiceState D_80043EB8[8];` (never
 * referenced by the body; they clashed with sequence_context.h's D_80043EB8 in the TU). No code change.
 * Shaping devices: none. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
extern u8 D_8002C630;
extern u32 func_80017644(u32);
u8 func_80018BEC(u32 identifier)
{
    if (D_8002C630) return func_80017644(identifier) != 0xFFFFFFFF;
    return 0;
}
