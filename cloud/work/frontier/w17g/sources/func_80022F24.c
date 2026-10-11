/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80022F24 (static, boot 0x80022F24, 232 B, segment 0x22300 -> src/rom/lib_22300.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 35 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_command_handler_80022F24.
 * Body + declarations verbatim from cloud/work/boot_tail/BT05-variables/nonmatch/func_80022F24.c (Astra / BT05-variables).
 * w17g TU-context edits (codegen-neutral, re-proved): private struct view renamed MacroState_80022F24 (the TU
 * already defines a different MacroState); func_80016E68 (id -> data table lookup, lib_16320) is declared `void *` here and in
 * the other w17g caller (the D10 files used two different pointee types); assignment converts, no code change.
 * Shaping devices: none. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState_80022F24 {
    u8 unknown00[0x24];
    u32 flags24;
    u8 unknown28[0x38];
    u32 identifier60;
} MacroState_80022F24;
typedef struct PackedEnvelope { u16 attack, decay, sustain, release; } PackedEnvelope;
#pragma pack()
typedef struct Envelope { u16 attack, decay, sustain, release; } Envelope;
extern void *func_80016E68(u16);
extern void func_800148F8(u32, const Envelope *);
u8 func_80022F24(MacroState_80022F24 *state, MacroCommand *command)
{
    Envelope envelope;
    PackedEnvelope *table;
    if ((table = func_80016E68(command->word[0] >> 8)) != 0) {
        envelope.attack = (table->attack >> 8) | (table->attack << 8);
        envelope.decay = (table->decay >> 8) | (table->decay << 8);
        envelope.sustain = (table->sustain >> 8) | (table->sustain << 8);
        envelope.release = (table->release >> 8) | (table->release << 8);
        func_800148F8(state->identifier60 & 0xFF, &envelope);
        state->flags24 |= 0x200;
    }
    return 0;
}
