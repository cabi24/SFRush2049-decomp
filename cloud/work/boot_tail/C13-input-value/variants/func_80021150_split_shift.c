/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Input-controller evaluation; see cloud/work/boot_tail/C13-input-value/README.md. */
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;

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

#pragma pack(1)
typedef struct ControlSource {
    u8 controller;
    u8 combine;
    s16 scale;
} ControlSource;
typedef struct ControlInput {
    ControlSource source[4];
    u8 count;
    u8 unknown11;
} ControlInput;
#pragma pack(0)

extern u16 func_80020A04(u8 controller, u8 channel, u8 set);

u16 func_80021150(VoiceState *voice, ControlInput *input)
{
    u32 i;
    u32 value;
    u8 controller;
    s32 tmp;
    s32 combined;

    for (value = 0, i = 0; i < input->count; ++i) {
        controller = input->source[i].controller;
        if (controller == 128 || controller == 1 || controller == 10 ||
            controller == 160 || controller == 161 || controller == 131 ||
            controller == 132) {
            switch (controller) {
            case 160:
                tmp = voice->lfo170;
                tmp = (s32)((u32)tmp << 1);
                break;
            case 161:
                tmp = voice->lfo17C;
                tmp = (s32)((u32)tmp << 1);
                break;
            default:
                tmp = func_80020A04(controller, voice->channel4A, voice->set4B) - 8192;
                break;
            }
            tmp = (s32)((u32)(s32)input->source[i].scale * (u32)tmp) >> 8;
            tmp = tmp < -8192 ? -8192 : tmp > 8191 ? 8191 : tmp;
            switch (input->source[i].combine) {
            case 0:
                combined = tmp;
                break;
            case 1:
                combined = (value + tmp) - 8192;
                combined = combined < -8192 ? -8192 : combined > 8191 ? 8191 : combined;
                break;
            case 2:
                combined = (s32)((value - 8192) * tmp) >> 13;
                combined = combined < -8192 ? -8192 : combined > 8191 ? 8191 : combined;
                break;
            }
            value = combined + 8192;
        } else {
            tmp = func_80020A04(controller, voice->channel4A, voice->set4B);
            tmp = (s32)((u32)(s32)input->source[i].scale * (u32)tmp) >> 8;
            if (tmp > 16383) tmp = 16383;
            switch (input->source[i].combine) {
            case 0:
                value = tmp;
                break;
            case 1:
                value += tmp;
                value = value > 16383 ? 16383 : value;
                break;
            case 2:
                value = (value * tmp) >> 14;
                value = value > 16383 ? 16383 : value;
                break;
            }
        }
    }
    return value;
}
