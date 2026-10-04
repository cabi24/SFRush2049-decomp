/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_8001F9D0.c: file-local type names VoiceState suffixed _8001F9D0 so several bodies share one ROM TU; no other change. */
typedef unsigned char u8;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState_8001F9D0 {
    u8 unknown00[36];
    u32 flags;
    u32 value28;
    u8 unknown2C[52];
    u32 identifier;
    u8 unknown64[89];
    u8 valueBD;
    u8 unknownBE[226];
} VoiceState_8001F9D0;
#pragma pack(0)
extern void func_8001EB10(VoiceState_8001F9D0 *);
extern void func_8001F6EC(VoiceState_8001F9D0 *);

void func_8001F9D0(VoiceState_8001F9D0 *state)
{
    func_8001EB10(state);
    state->flags &= ~3U;
    state->value28 = 0;
    func_8001F6EC(state);
}
