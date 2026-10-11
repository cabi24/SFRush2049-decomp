/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80023B50 - lib_22300 (IDO 7.1). Write macro variable INDEX of VOICE (the setter twin of
 * func_80023AD4): when IS_CONTROLLER, set the voice's MIDI controller (func_8002165C, lib_21210);
 * otherwise variable index & 31: 0..15 the voice's own s16 variables (+0x180), 16..31 the global
 * s16 variables at D_8004BE98.
 * Natural form: D_8004BE98[index - 16] = value (true=10 reloc_blind=0: relocation NAME only).
 * Relocation-label view (disclosed): written (&D_8004BE78[16])[index - 16], see func_80023AD4.
 * Types: as func_80023AD4; func_8002165C (lib_21210, unmatched) declared from its use.
 * Prior: D10 BT05-macro-five (5.3, 33 words; 7.1 true=10 reloc_blind=0). */
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
extern void func_8002165C(VoiceState *voice, u8 controller, s16 value);

void func_80023B50(VoiceVariables_80023AD4 *voice, u8 isController, u8 index, s16 value)
{
    if (isController) {
        func_8002165C((VoiceState *)voice, index, value);
    } else {
        index &= 31;
        if (index < 16) {
            voice->variables180[index] = value;
        } else {
            (&D_8004BE78[16])[index - 16] = value;
        }
    }
}
