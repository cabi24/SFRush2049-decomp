/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState {
    u8 unknown00[36];
    u32 flags;
    u32 value28;
    u8 unknown2C[52];
    u32 identifier;
    u8 unknown64[89];
    u8 valueBD;
    u8 unknownBE[226];
} VoiceState;
#pragma pack(0)
extern VoiceState D_8004BEB8[];
extern int func_8001467C(int);
extern void func_80014AF0(int);
extern void func_8001F6EC(VoiceState *);

void func_8001F954(int index)
{
    VoiceState *state;
    int active;
    if (index != -1) {
        active = func_8001467C(index);
        if (active) {
            func_80014AF0(index);
        }
        state = &D_8004BEB8[index];
        state->identifier = index;
        func_8001F6EC(state);
        state->valueBD = 0;
    }
}
