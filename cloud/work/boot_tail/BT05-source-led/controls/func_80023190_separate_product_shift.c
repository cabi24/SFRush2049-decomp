/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: donor-derived research control; see packet README. */
/* Source-family lead: AxioDL/musyx synthmacros.c at
 * 78d2e16e4905fc675952162d331c24d5198b2687, CC0-1.0.
 * N64 field offsets and zero byte return follow the native target.
 * The reference is PC/Dolphin source, not an authenticated N64 release.
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
    u8 unknown2C[2];
    u8 field2E;
    u8 unknown2F[9];
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
    s32 delta;
    s32 scale;
    delta = (state->field50 << 16) - ((u8)(command->word[0] >> 16) << 16);
    scale = (s8)((u8)(command->word[0] >> 8));
    delta *= scale;
    delta >>= 7;
    delta += (u8)(command->word[0] >> 24) << 16;
    delta = delta < 0 ? 0 : delta > 0x7F0000 ? 0x7F0000 : delta;
    state->field38 = delta;
    return 0;
}
