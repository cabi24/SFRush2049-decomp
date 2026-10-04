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

extern void func_8001EF8C(MacroState *, u8);

u8 func_80022514(MacroState *state, MacroCommand *command)
{
    s16 add;
    s16 priority;
    add = (u16)(command->word[0] >> 16);
    priority = state->field2E + add;
    priority = (priority < 0) ? 0 : (priority > 0xFF) ? 0xFF : priority;
    func_8001EF8C(state, priority);
    return 0;
}
