/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800217E4 - lib_22300 (IDO 7.1). Records the voice's id (low byte) in its channel map:
 * the per-set table D_80056160[set][channel], or D_800561E0[id] for set 255 (no set).
 * w19b: `index = voice->id60 & 0xFF` (no id local) is the 7.1 copy-then-narrow idiom: uopt CSEs
 * the id load (v0) with the -1 test and the mask becomes `move a1,v0; andi a1,a1,0xff` on a
 * coloured u8. A u32 id local + bare `index = id` gives `andi v1,v0` sunk into one arm (true=405).
 * VoiceState is the TU's packed typedef (lib_22300 lines 13-15) verbatim; pack(1) gives retail's lwl/lwr id load. No devices beyond the mask. */
typedef unsigned char u8;
typedef short s16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState { u8 unknown00[36]; u32 flags24; u8 unknown28[34]; u8 channel4A; u8 set4B; u8 unknown4C[20]; u32 id60; u8 unknown64[268]; s16 lfo170; u8 unknown172[10]; s16 lfo17C; u8 unknown17E[34]; } VoiceState;
#pragma pack(0)
extern u8 D_80056160[][16];
extern u8 D_800561E0[];

void func_800217E4(VoiceState *voice)
{
    u8 index;
    if (voice->id60 != 0xFFFFFFFFU && voice->channel4A != 255) {
        index = voice->id60 & 0xFF;
        if (voice->set4B == 255) {
            D_800561E0[index] = index;
        } else {
            D_80056160[voice->set4B][voice->channel4A] = index;
        }
    }
}
