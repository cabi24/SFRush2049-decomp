/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native macro helper reconstruction; only observed state layout is modeled. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState {
    u8 unknown00[0x24];
    u32 flags24;
    u8 unknown28[0x38];
    u32 identifier60;
} MacroState;
typedef struct PackedEnvelope { u16 attack, decay, sustain, release; } PackedEnvelope;
#pragma pack()
typedef struct Envelope { u16 attack, decay, sustain, release; } Envelope;
extern PackedEnvelope *func_80016E68(u16);
extern void func_800148F8(u32, const Envelope *);
u8 func_80022F24(MacroState *state, MacroCommand *command)
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
