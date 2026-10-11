/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001BDB8 (static, boot 0x8001BDB8, 92 B, segment 0x1a660 -> src/rom/lib_1a660.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 15 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_command_prepare_8001BDB8.
 * Body + declarations verbatim from cloud/work/boot_tail/BT03-high-medium/nonmatch/func_8001BDB8.c (Astra / BT03-high-medium).
 * Shaping devices: none. */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct ChannelState {
    u32 unknown00[2];
    int flags08;
    u32 pending0C;
    u32 unknown10;
    u8 kind14;
    u8 unknown15[19];
} ChannelState;
extern ChannelState D_8004F300[];
int func_8001BDB8(u8 index)
{
    ChannelState *state;
    state = &D_8004F300[index];
    if (state->kind14 != 4 && state->pending0C != 0 && state->flags08 < 0) return 1;
    return 0;
}
