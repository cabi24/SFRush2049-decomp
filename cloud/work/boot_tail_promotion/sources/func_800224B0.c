/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_800224B0.c: file-local type names MacroCommand, MacroState suffixed _800224B0 so several bodies share one ROM TU; no other change. */
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
typedef struct MacroState_800224B0 {
    u8 unknown00[0x28];
    u32 field28;
    u16 field2C;
    u8 unknown2E[2];
    u32 field30;
    u8 unknown34[4];
    s32 field38;
    u8 unknown3C[0x14];
    u16 field50;
} MacroState_800224B0;
#pragma pack(0)
typedef struct MacroCommand_800224B0 {
    u32 word[2];
} MacroCommand_800224B0;

u8 func_800224B0(MacroState_800224B0 *state, MacroCommand_800224B0 *command)
{
    u32 value;
    value = (u16)(command->word[0] >> 16)
          + (((command->word[1] & 0xFFFF) * (state->field30 >> 16)) >> 7);
    if (value > 60000) {
        state->field28 = 0x75300000;
    } else {
        state->field28 = value << 15;
    }
    return 0;
}
