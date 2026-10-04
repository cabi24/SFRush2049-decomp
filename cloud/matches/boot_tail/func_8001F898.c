/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState {
    u32 command00;
    u8 unknown04[12];
    u32 next_identifier;
    u8 unknown14[16];
    u32 flags24;
    u8 unknown28[36];
    u8 external4C;
    u8 unknown4D[19];
    u32 identifier60;
    u8 unknown64[89];
    u8 activeBD;
    u8 unknownBE[226];
} VoiceState;
#pragma pack(0)
extern VoiceState D_8004BEB8[];
extern int func_8001F13C(u8, u8, u16, u8);
extern void func_8001EB10(VoiceState *);
extern u8 func_8001467C(int);
extern void func_80014AF0(int);
extern void func_8001EF8C(VoiceState *, u8);
int func_8001F898(u8 channel)
{
    int index;
    VoiceState *state;
    index = func_8001F13C(channel, 255, 65535, 1);
    if (index != -1) {
        state = &D_8004BEB8[index];
        state->activeBD = 1;
        state->external4C = 1;
        func_8001EB10(state);
        state->identifier60 = (u32)index | 0xFFFFFF00U;
        if (func_8001467C(index)) func_80014AF0(index);
        state->command00 = 0;
        func_8001EF8C(state, channel);
    }
    return index;
}
