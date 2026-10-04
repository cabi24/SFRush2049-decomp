/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80021F68.c: file-local type names MacroCommand, MacroState suffixed _80021F68 so several bodies share one ROM TU; no other change. */
/* Native reconstruction of a two-argument audio macro handler. The packed
 * state offsets and command word are evidenced by the resident dispatcher;
 * original field names and the N64 middleware release remain unknown.
 * No authenticated arcade counterpart is available in this checkout.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#pragma pack(1)
typedef struct MacroState_80021F68 {
    u8 unknown00[0x1C];
    u32 field1C;
    u32 field20;
    u32 flags24;
    u32 field28;
    u8 unknown2C[0x3E];
    u8 field6A;
    u8 field6B;
    u8 unknown6C[0x2D];
    u8 field99;
    u8 field9A;
} MacroState_80021F68;
#pragma pack(0)

typedef struct MacroCommand_80021F68 {
    u32 word[2];
} MacroCommand_80021F68;

u8 func_80021F68(MacroState_80021F68 *state, MacroCommand_80021F68 *command)
{
    state->field1C = state->field20 = 0;
    return 0;
}
