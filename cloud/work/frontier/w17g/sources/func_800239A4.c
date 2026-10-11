/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800239A4 (static, boot 0x800239A4, 304 B, segment 0x22300 -> src/rom/lib_22300.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 21 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_command_handler_800239A4.
 * Body + declarations verbatim from cloud/work/boot_tail/BT05-larger-trio/nonmatch/func_800239A4.c (Astra / BT05-larger-trio).
 * w17g TU-context edits (codegen-neutral, re-proved): this file's private struct view is renamed MacroState_800239A4
 * (the TU already defines a different MacroState); func_8001E940 keeps the TU's prototype `extern void func_8001E940(u32 *, MacroState *);` with the TU's MacroState typedef copied verbatim, and the call casts `state`.
 * Shaping devices: the (MacroState *) argument cast above (type view only, no code).
 * Previously listed shaping devices: none. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState_800239A4 {
    u8 unknown00[0x24];
    u32 flags24;
    u8 unknown28[0x22];
    u8 channel4A;
    u8 set4B;
    u8 unknown4C[0x44];
    u32 time90;
    u8 unknown94[4];
    u8 mode98;
} MacroState_800239A4;
#pragma pack()
extern void func_8001E930(u32 *);
extern void func_80020FDC(u8, u8, u8);
extern void func_80019BE4(MacroState_800239A4 *);
typedef struct MacroState { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState;
extern void func_8001E940(u32 *, MacroState *);
u8 func_800239A4(MacroState_800239A4 *state, MacroCommand *command)
{
    u32 time;
    state->mode98 = command->word[0] >> 16;
    time = (u16)(command->word[1] >> 16);
    if ((u8)(command->word[1] >> 8) & 1) {
        func_8001E930(&time);
    } else {
        func_8001E940(&time, (MacroState *)state);
    }
    state->time90 = time;
    switch ((u8)(command->word[0] >> 8)) {
    case 0:
        state->flags24 &= ~0x800;
        state->flags24 |= 0x1000;
        if (state->channel4A != 255) func_80020FDC(state->channel4A, state->set4B, 0);
        break;
    case 1:
        if (!(state->flags24 & 0x800)) func_80019BE4(state);
        state->flags24 |= 0x1800;
        break;
    }
    return 0;
}
