/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* IDO 7.1, lib_22300: stop every active voice whose identifier equals the
 * command's macro/note prefix combined with its voice index. The count is
 * reloaded after func_80021700, which may change voice state. Return byte zero.
 * Reconstructed from the native body; no arcade ancestor is established.
 *
 * Shaping: the redundant u16 conversion of word[0]'s high halfword advances
 * ugen's temporary ring. as1 removes its mask and forwards the shift into the
 * later register. The u32 conversion before << 16 keeps unsigned semantics.
 * No unused locals, pad arrays, false prototypes or helper stand-ins.
 * VoiceState and MacroCommand are the existing lib_22300 declaration spellings;
 * the packed private argument view names only the halfword read at offset 0x4E.
 * Unknown byte intervals describe native object layout, not stack padding.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState { u8 unknown00[36]; u32 flags24; u8 unknown28[34]; u8 channel4A; u8 set4B; u8 unknown4C[20]; u32 id60; u8 unknown64[268]; s16 lfo170; u8 unknown172[10]; s16 lfo17C; u8 unknown17E[34]; } VoiceState;
#pragma pack(0)
extern VoiceState D_8004BEB8[];
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState_80022324 {
    u8 unknown00[0x4E];
    u16 keyGroup4E;
} MacroState_80022324;
#pragma pack(0)
extern u8 D_8004FA18;
extern int func_80021700(u32 id);
u8 func_80022324(MacroState_80022324 *voice, MacroCommand *command)
{
    u32 i;
    u32 id;
    id = (((command->word[0] >> 8) & 255) + voice->keyGroup4E) << 8;
    id |= (u32)(u16)(command->word[0] >> 16) << 16;
    for (i = 0; i < D_8004FA18; ++i) {
        if ((id | i) == D_8004BEB8[i].id60) {
            func_80021700(id | i);
        }
    }
    return 0;
}
