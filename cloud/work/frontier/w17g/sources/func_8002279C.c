/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8002279C (static, boot 0x8002279C, 216 B, segment 0x22300 -> src/rom/lib_22300.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 7 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_command_handler_8002279C.
 * Body + declarations verbatim from cloud/work/boot_tail/BT05-variables/nonmatch/func_8002279C.c (Astra / BT05-variables).
 * w17g TU-context edits (codegen-neutral, re-proved): this file's private struct view is renamed MacroState_8002279C
 * (the TU already defines a different MacroState); func_8002193C keeps the TU's prototype `extern u8 func_8002193C(MacroState *, MacroCommand *);` with the TU's MacroState typedef copied verbatim, and the call casts `state`.
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
typedef struct MacroState_8002279C {
    u8 unknown00[0x4A];
    u8 channel4A;
    u8 set4B;
    u8 unknown4C[4];
    u16 note50;
    u8 unknown52[0x6E];
    s8 detuneC0;
    u8 lastNoteC1;
} MacroState_8002279C;
#pragma pack()
extern void func_80020F4C(u8, u8, u8);
typedef struct MacroState { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState;
extern u8 func_8002193C(MacroState *, MacroCommand *);
u8 func_8002279C(MacroState_8002279C *state, MacroCommand *command)
{
    state->note50 = state->lastNoteC1 + (s8)(u8)(command->word[0] >> 8);
    state->note50 = (s16)state->note50 < 0 ? 0 : state->note50 > 127 ? 127 : state->note50;
    state->detuneC0 = command->word[0] >> 16;
    if (state->channel4A != 255) {
        func_80020F4C(state->channel4A, state->set4B, state->note50);
    }
    command->word[0] = 4;
    return func_8002193C((MacroState *)state, command);
}
