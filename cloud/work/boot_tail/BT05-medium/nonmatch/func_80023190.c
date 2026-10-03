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

u8 func_80023190(MacroState *state, MacroCommand *command)
{
    u32 parameter;
    s32 value;
    u32 gain;
    s32 limited;
    parameter = command->word[0];
    value = ((u32)state->field50 << 16)
          - (((parameter >> 16) & 0xFF) << 16);
    gain = (parameter >> 8) & 0xFF;
    gain = (s8)gain;
    value = (s32)((u32)value * gain);
    value >>= 7;
    value += (u8)(parameter >> 24) << 16;
    if (value < 0) {
        value = 0;
    } else {
        limited = value;
        if (value > 0x7F0000) limited = 0x7F0000;
        value = limited;
    }
    state->field38 = value;
    return 0;
}
