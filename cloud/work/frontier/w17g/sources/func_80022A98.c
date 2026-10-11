/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80022A98 (static, boot 0x80022A98, 448 B, segment 0x22300 -> src/rom/lib_22300.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 57 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_command_handler_80022A98.
 * Body + declarations verbatim from cloud/work/boot_tail/BT05-timed-pair/nonmatch/func_80022A98.c (Astra / BT05-timed-pair).
 * w17g TU-context edits (codegen-neutral, re-proved): this file's private struct view is renamed MacroState_80022A98
 * (the TU already defines a different MacroState); func_8001E940 keeps the TU's prototype `extern void func_8001E940(u32 *, MacroState *);` with the TU's MacroState typedef copied verbatim, and the call casts `state`.
 * Shaping devices: the (MacroState *) argument cast above (type view only, no code).
 * Previously listed shaping devices: none. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState_80022A98 {
    u8 unknown00[0x24];
    u32 flags24;
    u8 unknown28[0x44];
    u32 currentTime6C;
    u32 period70;
    u8 unknown74[4];
    u8 keyRange78;
    u8 centRange79;
} MacroState_80022A98;
#pragma pack()
extern void func_8001E930(u32 *);
typedef struct MacroState { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState;
extern void func_8001E940(u32 *, MacroState *);
u8 func_80022A98(MacroState_80022A98 *state, MacroCommand *command)
{
    u32 time;
    s8 keyRange;
    s8 centRange;
    if ((u8)(command->word[0] >> 24) & 3) {
        state->flags24 |= 0x8000;
    } else {
        state->flags24 &= ~0x8000;
    }
    time = (u16)(command->word[1] >> 16);
    if ((u8)(command->word[1] >> 8) & 1) {
        func_8001E930(&time);
    } else {
        func_8001E940(&time, (MacroState *)state);
    }
    if (time) {
        state->flags24 |= 0x4000;
        state->period70 = time;
        keyRange = command->word[0] >> 8;
        centRange = command->word[0] >> 16;
        if (keyRange < 0) {
            if (centRange < 0) state->centRange79 = -centRange;
            else state->centRange79 = centRange;
            state->keyRange78 = -keyRange;
            state->currentTime6C = state->period70 / 2;
        } else {
            if (centRange < 0) {
                if (keyRange == 0) {
                    state->centRange79 = -centRange;
                    state->currentTime6C = state->period70 / 2;
                } else {
                    --keyRange;
                    state->centRange79 = 100 - centRange;
                    state->currentTime6C = 0;
                }
            } else {
                state->centRange79 = centRange;
                state->currentTime6C = 0;
            }
            state->keyRange78 = keyRange;
        }
    } else {
        state->flags24 &= ~0x4000;
    }
    return 0;
}
