/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native macro handler; see BT05-macro-five/README.md for ABI and provenance. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned long u32;
typedef long s32;
typedef struct MacroCommand { u32 word0; u32 word1; } MacroCommand;

#pragma pack(1)
typedef struct VoiceState {
    MacroCommand *program;
    MacroCommand *current;
    u8 unknown08[28];
    u32 flags24;
    u8 unknown28[16];
    u32 pan38;
    u32 panDelta3C;
    u8 unknown40[14];
    u16 keyGroup4E;
    u8 unknown50[16];
    u32 id60;
    u8 unknown64[68];
    u32 panTimeA8;
    u32 panTargetAC;
    u8 unknownB0[208];
    s16 variables180[16];
} VoiceState;
#pragma pack(0)

extern void func_8001E930(u32 *time);
u8 func_800230B0(VoiceState *voice, MacroCommand *command)
{
    u32 time;
    s32 offset;
    voice->flags24 |= 0x10000;
    time = (command->word0 >> 16) & 65535;
    voice->panTimeA8 = time;
    func_8001E930(&voice->panTimeA8);
    voice->pan38 = ((command->word0 >> 8) & 255) << 16;
    offset = (s32)(command->word1 << 24) >> 24;
    voice->panTargetAC = ((u32)offset << 16) + voice->pan38;
    if (voice->panTimeA8 != 0) {
        voice->panDelta3C = ((u32)offset << 16) / time;
    } else {
        voice->panDelta3C = (u32)offset << 16;
    }
    return 0;
}
