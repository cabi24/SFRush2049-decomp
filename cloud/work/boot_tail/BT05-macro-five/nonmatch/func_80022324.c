/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native macro handler; see BT05-macro-five/README.md for ABI and provenance. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
typedef int s32;
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

extern VoiceState D_8004BEB8[];
extern u8 D_8004FA18;
extern int func_80021700(u32 id);
u8 func_80022324(VoiceState *voice, MacroCommand *command)
{
    u32 i;
    u32 id;
    id = (((command->word0 >> 8) & 255) + voice->keyGroup4E) << 8;
    id |= (command->word0 >> 16) << 16;
    for (i = 0; i < D_8004FA18; ++i) {
        if ((id | i) == D_8004BEB8[i].id60) {
            func_80021700(id | i);
        }
    }
    return 0;
}
