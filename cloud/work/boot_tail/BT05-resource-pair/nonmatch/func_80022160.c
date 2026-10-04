/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native resource macro reconstruction; all callee inputs are genuine. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState {
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
} MacroState;
#pragma pack()
extern MacroState D_8004BEB8[];
extern u32 func_80024988(u32, u16, u8, u8, u8, u8, u8, u16, u16, u8, u8);
extern void func_8001B744(MacroState *, MacroState *);
u8 func_80022160(MacroState *state, MacroCommand *command)
{
    u32 macro;
    s32 key;
    u32 child;
    macro = (u8)(command->word[1] >> 24) | ((command->word[0] >> 16) << 16) |
        ((u8)(command->word[1] >> 16) << 8);
    key = state->originalNote4E + (s8)(u8)(command->word[0] >> 8);
    key = key < 0 ? 0 : key > 127 ? 127 : key;
    if (state->external4C) key |= 128;
    state->activeBD = 1;
    child = func_80024988(macro, state->allocationBA, key,
        state->volume30 >> 16, state->panning38 >> 16, state->channel4A,
        state->set4B, command->word[1], state->section2F, 0, state->groupBE);
    state->activeBD = 0;
    if (child != 0xFFFFFFFFU) {
        D_8004BEB8[(u8)child].parent14 = state->identifier60;
        if (state->child10 != 0xFFFFFFFFU) {
            D_8004BEB8[(u8)child].child10 = state->child10;
            D_8004BEB8[(u8)state->child10].parent14 = child;
        }
        state->child10 = child;
        if (state->external4C) func_8001B744(&D_8004BEB8[(u8)child], state);
    }
    return 0;
}
