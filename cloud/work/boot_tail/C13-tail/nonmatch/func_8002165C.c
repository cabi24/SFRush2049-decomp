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

extern u8 func_80021548(u8 controller);
extern void func_800206AC(u8 controller, u8 channel, u8 set, u16 value);

void func_8002165C(VoiceState *voice, u8 controller, s16 value)
{
    value = value < 0 ? 0 : value > 16383 ? 16383 : value;
    switch (func_80021548(controller)) {
    case 160:
    case 161:
        break;
    default:
        if (voice->channel4A != 255) {
            func_800206AC(controller, voice->channel4A, voice->set4B, value);
        }
        break;
    }
}
