/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80021764 - lib_22300 (IDO 7.1). Returns 1 when the voice's channel map entry still holds
 * this voice's id (low byte), else 0 (the test half of func_800217E4 / func_80021844).
 * w19b: same `index = voice->id60 & 0xFF` copy-then-narrow idiom as func_800217E4.
 * VoiceState is the TU's typedef verbatim. */
typedef unsigned char u8;
typedef short s16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState { u8 unknown00[36]; u32 flags24; u8 unknown28[34]; u8 channel4A; u8 set4B; u8 unknown4C[20]; u32 id60; u8 unknown64[268]; s16 lfo170; u8 unknown172[10]; s16 lfo17C; u8 unknown17E[34]; } VoiceState;
#pragma pack(0)
extern u8 D_80056160[][16];
extern u8 D_800561E0[];
struct MacroState;

int func_80021764(struct MacroState *state)
{
    u8 index;
    if (((VoiceState *)state)->id60 != 0xFFFFFFFFU && ((VoiceState *)state)->channel4A != 255) {
        index = ((VoiceState *)state)->id60 & 0xFF;
        if (((VoiceState *)state)->set4B == 255) {
            if (D_800561E0[index] == index) return 1;
        } else {
            if (D_80056160[((VoiceState *)state)->set4B][((VoiceState *)state)->channel4A] == index) return 1;
        }
    }
    return 0;
}
