/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native controller helper; see cloud/work/boot_tail/C13-tail/README.md. */
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef unsigned int u32;

#pragma pack(1)
typedef struct VoiceState {
    u8 unknown00[36];
    u32 flags24;
    u8 unknown28[34];
    u8 channel4A;
    u8 set4B;
    u8 unknown4C[20];
    u32 id60;
    u8 unknown64[268];
    s16 lfo170;
    u8 unknown172[10];
    s16 lfo17C;
    u8 unknown17E[34];
} VoiceState;
#pragma pack(0)

extern u8 D_80056160[][16];
extern u8 D_800561E0[];

void func_800217E4(VoiceState *voice)
{
    u32 index;
    if (voice->id60 != 0xFFFFFFFFU && voice->channel4A != 255) {
        index = voice->id60;
        index &= 255;
        if (voice->set4B == 255) {
            D_800561E0[index] = index;
        } else {
            D_80056160[voice->set4B][voice->channel4A] = index;
        }
    }
}
