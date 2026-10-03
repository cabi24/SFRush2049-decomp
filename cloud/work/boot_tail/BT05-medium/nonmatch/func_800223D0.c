/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: bounded allocation/source-form residual. */
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
typedef struct MacroState {
    u8 unknown00[0x28];
    u32 field28;
    u16 field2C;
    u8 unknown2E[2];
    u32 field30;
    u8 unknown34[4];
    s32 field38;
    u8 unknown3C[0x14];
    u16 field50;
} MacroState;
#pragma pack(0)
typedef struct MacroCommand {
    u32 word[2];
} MacroCommand;

u8 func_800223D0(MacroState *state, MacroCommand *command)
{
    s32 delta;
    u32 parameter;
    s32 value;
    parameter = command->word[0];
    delta = (s16)(parameter >> 16);
    value = (state->field28 >> 15) + delta;
    if (value < 0) {
        state->field28 = 0;
    } else if (value > 65535) {
        state->field28 = 0x7FFF8000;
    } else {
        state->field28 = (u32)value << 15;
    }
    return 0;
}
