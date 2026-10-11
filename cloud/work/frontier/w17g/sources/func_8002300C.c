/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8002300C (static, boot 0x8002300C, 164 B, segment 0x22300 -> src/rom/lib_22300.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 22 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_command_handler_8002300C.
 * Body + declarations verbatim from cloud/work/boot_tail/BT05-variables/nonmatch/func_8002300C.c (Astra / BT05-variables).
 * w17g TU-context edits (codegen-neutral, re-proved): this file's private struct view is renamed MacroState_8002300C
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
typedef struct MacroState_8002300C {
    u8 unknown00[0x40];
    u32 sweepOffset40[2];
    u8 sweepNumber48[2];
    u8 unknown4A[0x32];
    u32 sweepAddition7C[2];
    u8 unknown84[0x2C];
    u8 sweepCountB0[2];
} MacroState_8002300C;
extern u32 func_80014D30(u32);
typedef struct MacroState { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState;
extern u8 func_8002193C(MacroState *, MacroCommand *);
u8 func_8002300C(MacroState_8002300C *state, MacroCommand *command, int index)
{
    s32 delta;
    state->sweepOffset40[index] = 0;
    state->sweepNumber48[index] = (u8)(command->word[0] >> 8);
    state->sweepCountB0[index] = state->sweepNumber48[index];
    delta = (s16)(command->word[0] >> 16);
    if (delta >= 0) {
        delta = (s32)func_80014D30(delta);
    } else {
        delta = (s32)(0U - func_80014D30(-delta));
    }
    state->sweepAddition7C[index] = (u32)delta << 16;
    command->word[0] = 0;
    return func_8002193C((MacroState *)state, command);
}
