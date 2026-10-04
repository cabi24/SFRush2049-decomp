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

extern s16 D_8004BE98[16];
extern void func_8002165C(VoiceState *voice, u8 controller, s16 value);
void func_80023B50(voice, isController, index, value)
VoiceState *voice;
u8 isController;
u8 index;
s16 value;
{
    if (isController) {
        func_8002165C(voice, index, value);
    } else {
        index &= 31;
        if (index < 16) {
            voice->variables180[index] = value;
        } else {
            D_8004BE98[index - 16] = value;
        }
    }
}
