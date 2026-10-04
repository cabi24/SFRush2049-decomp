/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Packed controller initialization; see cloud/work/boot_tail/C13-medium/README.md. */
typedef unsigned char u8;
typedef unsigned short u16;

#pragma pack(1)
typedef struct ControlSource {
    u8 controller;
    u8 combine;
    u16 scale;
} ControlSource;

typedef struct ControlInput {
    ControlSource source[4];
    u8 count;
    u8 unknown11;
} ControlInput;

typedef struct VoiceControls {
    u8 unknown00[196];
    ControlInput input[9];
} VoiceControls;
#pragma pack(0)

void func_8002106C(VoiceControls *voice)
{
    voice->input[0].source[0].controller = 7;
    voice->input[0].source[0].combine = 0;
    voice->input[0].source[0].scale = 256;
    voice->input[0].count = 1;
    voice->input[1].source[0].controller = 10;
    voice->input[1].source[0].combine = 0;
    voice->input[1].source[0].scale = 256;
    voice->input[1].count = 1;
    voice->input[2].source[0].controller = 131;
    voice->input[2].source[0].combine = 0;
    voice->input[2].source[0].scale = 256;
    voice->input[2].count = 1;
    voice->input[3].source[0].controller = 128;
    voice->input[3].source[0].combine = 0;
    voice->input[3].source[0].scale = 256;
    voice->input[3].count = 1;
    voice->input[5].source[0].controller = 1;
    voice->input[5].source[0].combine = 0;
    voice->input[5].source[0].scale = 256;
    voice->input[5].count = 1;
    voice->input[6].source[0].controller = 64;
    voice->input[6].source[0].combine = 0;
    voice->input[6].source[0].scale = 256;
    voice->input[6].count = 1;
    voice->input[7].source[0].controller = 65;
    voice->input[7].source[0].combine = 0;
    voice->input[7].source[0].scale = 256;
    voice->input[7].count = 1;
    voice->input[8].source[0].controller = 91;
    voice->input[8].source[0].combine = 0;
    voice->input[8].source[0].scale = 256;
    voice->input[8].count = 1;
    voice->input[4].source[0].controller = 132;
    voice->input[4].source[0].combine = 0;
    voice->input[4].source[0].scale = 256;
    voice->input[4].count = 1;
}
