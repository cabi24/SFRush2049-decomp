/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80021150 - lib_21210 (IDO 7.1). Evaluate a voice's control input: for each of the
 * input's COUNT sources, read the controller value (func_80020A04 for channel 0x4A / set 0x4B,
 * or the voice's LFOs at +0x170 / +0x17C for controllers 160 / 161, doubled), scale it by the
 * source's signed 8.8 SCALE and combine it into the running value (mode 0 replace, 1 add,
 * 2 multiply). The seven "bipolar" controllers (128, 1, 10, 160, 161, 131, 132) work in
 * -8192..8191 around 8192; the rest are unipolar and clamp at 16383. Returns the value.
 * Callers: func_80021428 .. func_80021528 (lib_21210, locked) with VOICE + 196.
 * Types: the TU's ControlSource/ControlInput (func_8002106C's block, verbatim, deduplicated
 * by promote_batch --prefix-dedup); VoiceState_80021150 = the TU's later packed VoiceState
 * (func_800215A8's block) under a private name, since that typedef follows this slot.
 * The parameters are `void *` as the TU's later prototype (func_80021428's block) declares
 * them; each access casts to the view (type view only: no code; typed locals would cost
 * frame slots). `(s16)scale` is the signed reading retail performs (lb high byte).
 * w19a: from D10 C13 (5.3; under 7.1 true=410): `tmp += tmp` -> `* 2` (sll, as
 * func_800215A8) and `(value + tmp) - 8192` -> `value - 8192 + tmp` (addu operand order).
 * As in retail, `combined` is not initialised for an unsupported first combine mode.
 * No shaping devices, no unused locals. */
#include "types.h"
#pragma pack(1)
typedef struct ControlSource { u8 controller; u8 combine; u16 scale; } ControlSource;
typedef struct ControlInput { ControlSource source[4]; u8 count; u8 unknown11; } ControlInput;
typedef struct VoiceState_80021150 { u8 unknown00[36]; u32 flags24; u8 unknown28[34]; u8 channel4A; u8 set4B; u8 unknown4C[20]; u32 id60; u8 unknown64[268]; s16 lfo170; u8 unknown172[10]; s16 lfo17C; u8 unknown17E[34]; } VoiceState_80021150;
#pragma pack(0)
extern u16 func_80020A04(u8 controller, u8 channel, u8 set);

u16 func_80021150(void *voice, void *control)
{
    u32 i;
    u32 value;
    u8 controller;
    s32 tmp;
    s32 combined;

    for (value = 0, i = 0; i < ((ControlInput *)control)->count; ++i) {
        controller = ((ControlInput *)control)->source[i].controller;
        if (controller == 128 || controller == 1 || controller == 10 ||
            controller == 160 || controller == 161 || controller == 131 ||
            controller == 132) {
            switch (controller) {
            case 160:
                tmp = ((VoiceState_80021150 *)voice)->lfo170 * 2;
                break;
            case 161:
                tmp = ((VoiceState_80021150 *)voice)->lfo17C * 2;
                break;
            default:
                tmp = func_80020A04(controller, ((VoiceState_80021150 *)voice)->channel4A, ((VoiceState_80021150 *)voice)->set4B) - 8192;
                break;
            }
            tmp = (s32)((s16)((ControlInput *)control)->source[i].scale * tmp) >> 8;
            tmp = tmp < -8192 ? -8192 : tmp > 8191 ? 8191 : tmp;
            switch (((ControlInput *)control)->source[i].combine) {
            case 0:
                combined = tmp;
                break;
            case 1:
                combined = value - 8192 + tmp;
                combined = combined < -8192 ? -8192 : combined > 8191 ? 8191 : combined;
                break;
            case 2:
                combined = (s32)((value - 8192) * tmp) >> 13;
                combined = combined < -8192 ? -8192 : combined > 8191 ? 8191 : combined;
                break;
            }
            value = combined + 8192;
        } else {
            tmp = func_80020A04(controller, ((VoiceState_80021150 *)voice)->channel4A, ((VoiceState_80021150 *)voice)->set4B);
            tmp = (s32)((s16)((ControlInput *)control)->source[i].scale * tmp) >> 8;
            if (tmp > 16383) tmp = 16383;
            switch (((ControlInput *)control)->source[i].combine) {
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
