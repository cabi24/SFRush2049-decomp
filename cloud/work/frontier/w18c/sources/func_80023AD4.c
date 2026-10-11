/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80023AD4 - lib_22300 (IDO 7.1). Read macro variable INDEX of VOICE: when IS_CONTROLLER,
 * the voice's MIDI controller value (func_800215A8, lib_21210, locked); otherwise variable
 * index & 31, where 0..15 are the voice's own s16 variables (+0x180) and 16..31 the 16 global
 * s16 variables at D_8004BE98.
 * Natural form: D_8004BE98[index - 16] (scores true=10 reloc_blind=0: relocation NAME only).
 * Relocation-label view (disclosed shaping device): IDO folds D_8004BE98[index - 16] into
 * (D_8004BE98 - 32)[index], an address the reloc-aware target names by its own label
 * D_8004BE78, so the globals are spelled (&D_8004BE78[16])[index - 16] (= D_8004BE98[index - 16];
 * written D_8004BE78[index] the index copy disappears, true=300); D_8004BE78 is declared here as an
 * s16 array view (lib_17dc0's sequence_context.h declares the word at that address
 * as unsigned int; different TU). Same practice as docs/reloc_reconciliation.md.
 * Types: the TU's packed VoiceState (line 14, verbatim) for func_800215A8's locked prototype; the
 * variables are read through a private packed view (VoiceVariables_80023AD4: type view only).
 * Prior: D10 BT05-macro-five (5.3, 25 words; 7.1 true=10 reloc_blind=0). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef int s32;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState { u8 unknown00[36]; u32 flags24; u8 unknown28[34]; u8 channel4A; u8 set4B; u8 unknown4C[20]; u32 id60; u8 unknown64[268]; s16 lfo170; u8 unknown172[10]; s16 lfo17C; u8 unknown17E[34]; } VoiceState;
typedef struct VoiceVariables_80023AD4 { u8 unknown000[0x180]; s16 variables180[16]; } VoiceVariables_80023AD4;
#pragma pack(0)
extern s16 D_8004BE78[];
extern u16 func_800215A8(VoiceState *voice, u8 controller);

s16 func_80023AD4(VoiceVariables_80023AD4 *voice, u8 isController, u8 index)
{
    s32 value;
    if (isController) {
        return func_800215A8((VoiceState *)voice, index);
    }
    index &= 31;
    value = index < 16 ? voice->variables180[index] : (&D_8004BE78[16])[index - 16];
    return value;
}
