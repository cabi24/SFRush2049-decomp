/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80022160 - lib_22300 (IDO 7.1). Macro command "start child macro" (MusyX macStart from a
 * running voice): macro id = word[0] >> 16, priority = word[1] bits 16..23, limit = word[1] bits
 * 24..31; key = original note + signed word[0] bits 8..15, clamped to 0..127, bit 7 set for an
 * external voice; starts it with func_80024988 (the voice's allocation, volume, panning, channel,
 * set, section, group; offset = word[1]; while activeBD is set) and links the new voice as this
 * voice's child (parent14 / child10 chain through D_8004BEB8). External voices also copy this
 * voice's state (func_8001B744). Returns 0.
 * w20b (from D10 BT05-resource-pair, 7.1 true=1130): macro written id-first with the id as
 * `(u16)(word[0] >> 16) << 16` (the u16 conversion is the one extra ugen temp in retail's ring;
 * without it every temp is one register early, true=320); indices of D_8004BEB8 as `x & 255`
 * (retail re-reads the whole child10 word and narrows it; `(u8)` folds to lbu); locals declared
 * key, macro, child (retail's macro spill slot +64). No devices beyond the masks, no unused locals.
 * Types: the TU's VoiceState D_8004BEB8 addressed through the packed private view
 * MacroState_80022160 (D10's MacroState) by cast; the TU's MacroCommand; func_80024988 as its locked
 * definition. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
typedef signed int s32;
#pragma pack(1)
typedef struct VoiceState { u8 unknown00[36]; u32 flags24; u8 unknown28[34]; u8 channel4A; u8 set4B; u8 unknown4C[20]; u32 id60; u8 unknown64[268]; s16 lfo170; u8 unknown172[10]; s16 lfo17C; u8 unknown17E[34]; } VoiceState;
#pragma pack(0)
extern VoiceState D_8004BEB8[];
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState_80022160 {
    u8 unknown00[0x10];
    u32 child10;
    u32 parent14;
    u8 unknown18[0x17];
    u8 section2F;
    u32 volume30;
    u8 unknown34[4];
    u32 panning38;
    u8 unknown3C[0xE];
    u8 channel4A;
    u8 set4B;
    u8 external4C;
    u8 unknown4D;
    u16 originalNote4E;
    u8 unknown50[0x10];
    u32 identifier60;
    u8 unknown64[0x56];
    u16 allocationBA;
    u8 unknownBC;
    u8 activeBD;
    u8 groupBE;
    u8 unknownBF[0xE1];
} MacroState_80022160;
#pragma pack()
extern u32 func_80024988(u32 packed, u16 allocation, u8 key, u8 volume,
                 u8 panning, u8 channel, u8 set, u16 offset,
                 u16 section, u8 start, u8 group);
extern void func_8001B744(MacroState_80022160 *, MacroState_80022160 *);
u8 func_80022160(MacroState_80022160 *state, MacroCommand *command)
{
    s32 key;
    u32 macro;
    u32 child;
    macro = (u16)(command->word[0] >> 16) << 16 | ((command->word[1] >> 16) & 0xFF) << 8 | ((command->word[1] >> 24) & 0xFF);
    key = state->originalNote4E + (s8)(u8)(command->word[0] >> 8);
    key = key < 0 ? 0 : key > 127 ? 127 : key;
    if (state->external4C) key |= 128;
    state->activeBD = 1;
    child = func_80024988(macro, state->allocationBA, key,
        state->volume30 >> 16, state->panning38 >> 16, state->channel4A,
        state->set4B, command->word[1], state->section2F, 0, state->groupBE);
    state->activeBD = 0;
    if (child != 0xFFFFFFFFU) {
        ((MacroState_80022160 *)&D_8004BEB8[child & 255])->parent14 = state->identifier60;
        if (state->child10 != 0xFFFFFFFFU) {
            ((MacroState_80022160 *)&D_8004BEB8[child & 255])->child10 = state->child10;
            ((MacroState_80022160 *)&D_8004BEB8[state->child10 & 255])->parent14 = child;
        }
        state->child10 = child;
        if (state->external4C) func_8001B744((MacroState_80022160 *)&D_8004BEB8[child & 255], state);
    }
    return 0;
}
