/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native resource macro reconstruction; all callee inputs are genuine. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
typedef struct SampleInfo {
    u32 info;
    u32 address;
    u32 offset;
    u32 length;
    u32 loopStart;
    u32 loopLength;
    u8 type;
} SampleInfo;
#pragma pack(1)
typedef struct MacroState {
    u8 unknown00[0x24];
    u32 flags24;
    u8 unknown28[8];
    u32 volume30;
    u8 unknown34[0x24];
    u32 sampleAddress58;
    u32 sampleInfo5C;
    u32 identifier60;
} MacroState;
#pragma pack()
extern SampleInfo D_80056208;
extern int func_80016CF0(u16, SampleInfo *);
extern void func_800146B4(u32, SampleInfo *, u8);
u8 func_80022874(MacroState *state, MacroCommand *command)
{
    if (func_80016CF0(command->word[0] >> 8, &D_80056208) == 0) {
        switch ((u8)(command->word[0] >> 24)) {
        case 0:
            D_80056208.offset = command->word[1];
            break;
        case 1:
            D_80056208.offset = (command->word[1] * (127U - (state->volume30 >> 16))) / 127U;
            break;
        case 2:
            D_80056208.offset = (command->word[1] * (state->volume30 >> 16)) / 127U;
            break;
        default:
            D_80056208.offset = 0;
        }
        if (D_80056208.offset >= D_80056208.length) {
            D_80056208.offset = D_80056208.length - 1;
        }
        func_800146B4(state->identifier60 & 255, &D_80056208, (state->flags24 & 0x200) == 0);
        state->sampleInfo5C = D_80056208.info;
        state->sampleAddress58 = D_80056208.address;
        state->flags24 |= 0x20;
    }
    return 0;
}
