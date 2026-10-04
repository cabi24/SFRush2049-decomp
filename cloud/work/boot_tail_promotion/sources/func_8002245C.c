/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_8002245C.c: file-local type names MacroCommand, MacroState suffixed _8002245C so several bodies share one ROM TU; no other change. */
/* Native macro-state reconstruction; exact field names and N64 middleware
 * release remain unknown. No authenticated arcade source is available here.
 */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
#pragma pack(1)
typedef struct MacroState_8002245C {
    u8 unknown00[0x28];
    u32 field28;
    u16 field2C;
    u8 unknown2E[2];
    u32 field30;
    u8 unknown34[4];
    s32 field38;
    u8 unknown3C[0x14];
    u16 field50;
} MacroState_8002245C;
#pragma pack(0)
typedef struct MacroCommand_8002245C {
    u32 word[2];
} MacroCommand_8002245C;

u8 func_8002245C(MacroState_8002245C *state, MacroCommand_8002245C *command)
{
    u32 divisor;
    divisor = command->word[1];
    if (divisor != 0) {
        state->field2C = (state->field28 >> 8) / divisor;
    } else {
        state->field2C = 0;
    }
    return 0;
}
