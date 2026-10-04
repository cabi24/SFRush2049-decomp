/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native core macro reconstruction; genuine helper and storage contracts. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState {
    MacroCommand *start00;
    MacroCommand *current04;
    MacroCommand *saved08;
    u8 unknown0C[4];
    u32 child10;
    u32 parent14;
    u8 unknown18[12];
    u32 flags24;
    u32 age28;
    u16 ageSpeed2C;
    u8 unknown2E[0x1E];
    u8 external4C;
    u8 unknown4D;
    u16 originalNote4E;
    u16 note50;
    u8 volume52;
    u8 panning53;
    u8 channel54;
    u8 set55;
    u8 section56;
    u8 unknown57[9];
    u32 identifier60;
    u16 macro64;
    u8 unknown66[0x36];
    u32 deadline9C;
    u8 unknownA0[0x1A];
    u16 allocationBA;
    u8 unknownBC[3];
    u8 groupBF;
    u8 detuneC0;
    u8 unknownC1[0xDF];
} MacroState;
#pragma pack()
extern MacroState D_8004BEB8[];
extern MacroCommand *func_80016C20(u16);
extern int func_8001F13C(u8, u8, u16, u8);
extern void func_8001EB10(MacroState *);
extern u8 func_8001467C(u32);
extern void func_80020820(u8, u8);
extern void func_8001EF8C(MacroState *, u8);
extern u32 func_8001ECE0(MacroState *);
u32 func_80024988(u32 packed, u16 allocation, u8 key, u8 volume,
                 u8 panning, u8 channel, u8 set, u16 offset,
                 u16 section, u8 start, u8 group)
{
    MacroCommand *program;
    MacroState *state;
    u32 macro;
    u32 external;
    u8 priority;
    int index;
    macro = packed >> 16;
    program = func_80016C20(macro);
    if (program != 0) {
        priority = packed >> 8;
        external = key & 128;
        index = func_8001F13C(priority, packed, allocation, external ? 1 : 0);
        if (index != -1) {
            state = &D_8004BEB8[index];
            func_8001EB10(state);
            state->flags24 = (state->flags24 & 0x10) | 2;
            if (func_8001467C(index)) state->flags24 |= 1;
            state->deadline9C = 0;
            if (external) {
                state->external4C = 1;
                key &= 127;
                func_80020820(index, 255);
                state->channel54 = index;
                state->set55 = 255;
            } else {
                state->external4C = 0;
                state->channel54 = channel;
                state->set55 = set;
            }
            state->macro64 = macro;
            state->allocationBA = allocation;
            state->age28 = 0x75300000U;
            state->ageSpeed2C = 1024;
            state->start00 = program;
            state->current04 = program + offset;
            state->originalNote4E = key;
            state->note50 = key;
            state->detuneC0 = 0;
            state->volume52 = volume;
            state->panning53 = panning;
            state->saved08 = 0;
            state->child10 = 0xFFFFFFFFU;
            state->parent14 = 0xFFFFFFFFU;
            state->section56 = section;
            state->groupBF = group;
            state->identifier60 = (macro << 16) | ((u32)key << 8) | (u32)index;
            func_8001EF8C(state, priority);
            if (start) return func_8001ECE0(state);
            return state->identifier60;
        }
    }
    return 0xFFFFFFFFU;
}
