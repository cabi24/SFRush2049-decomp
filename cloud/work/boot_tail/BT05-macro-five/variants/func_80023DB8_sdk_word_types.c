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

extern s16 func_80023AD4(VoiceState *voice, u8 isController, u8 index);
/* Known dispatcher callers supply comparison 0 or 1. Other values have no
 * defined result in the original handler and are outside this command ABI. */
u8 func_80023DB8(VoiceState *voice, MacroCommand *command, u8 comparison)
{
    s16 left;
    s16 right;
    u8 condition;
    left = func_80023AD4(voice, command->word0 >> 8, command->word0 >> 16);
    right = func_80023AD4(voice, command->word0 >> 24, command->word1);
    switch (comparison) {
    case 0: condition = left == right; break;
    case 1: condition = left < right; break;
    }
    if ((command->word1 >> 8) & 255) {
        condition = !condition;
    }
    if (condition) {
        voice->current = voice->program + ((command->word1 >> 16) & 65535);
    }
    return 0;
}
