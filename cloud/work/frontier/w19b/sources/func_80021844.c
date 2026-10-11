/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80021844 - lib_22300 (IDO 7.1). Releases the voice's channel map entry: if it still holds
 * this voice's id (low byte), it is reset to 255 (free).
 * w19b: same `index = voice->id60 & 0xFF` copy-then-narrow idiom as func_800217E4; the entry
 * address is CSE'd between the test and the store (lui/addiu + addu in retail).
 * VoiceState is the TU's typedef verbatim. */
typedef unsigned char u8;
typedef short s16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState { u8 unknown00[36]; u32 flags24; u8 unknown28[34]; u8 channel4A; u8 set4B; u8 unknown4C[20]; u32 id60; u8 unknown64[268]; s16 lfo170; u8 unknown172[10]; s16 lfo17C; u8 unknown17E[34]; } VoiceState;
#pragma pack(0)
extern u8 D_80056160[][16];
extern u8 D_800561E0[];

void func_80021844(VoiceState *voice)
{
    u8 index;
    if (voice->id60 != 0xFFFFFFFFU && voice->channel4A != 255) {
        index = voice->id60 & 0xFF;
        if (voice->set4B == 255) {
            if (D_800561E0[index] == index) D_800561E0[index] = 255;
        } else {
            if (D_80056160[voice->set4B][voice->channel4A] == index) D_80056160[voice->set4B][voice->channel4A] = 255;
        }
    }
}
