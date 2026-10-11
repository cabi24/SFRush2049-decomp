/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* func_80011A64 - lib_11640 (IDO 7.1). Per-buffer envelope step of one native audio voice
 * (AudioState, the TU's boot_tail_audio_record.h, 0x68-byte records): the step increment is
 * samples * 32000 / output rate (D_8003828C) in 16.16 << 11; a voice whose key is released
 * (pending == 0) enters release (state 3: restart the step, save the current level, target 0,
 * release_count); otherwise, when the step passes count, the attack state 0 moves to decay
 * (state 1: level 1.0 -> sustain value24 over value22 steps), decay moves to sustain (state 2,
 * level fixed, return) and a finished release clears *active and silences the voice (return).
 * The level is interpolated saved_value -> scale by step / (count * 65536).
 * Field names are the header's (pending 0x01, value22 0x22, value24 0x24); no private view.
 * FLAGS: the match needs as1's -r4300_mul (-Wab,-r4300_mul), which the Makefile passes to every
 * ROM TU (CFLAGS) and context_check.py uses; it stops as1 putting a multiply in a branch delay
 * slot. Without it as1 moves the mul.s into the `bgez step` delay slot and renames 65536.0f to
 * $f14 (true=775, the w19a/w20a "hoisting" residual). The lock flagset therefore carries
 * -Wab,-r4300_mul (see w21a/RESULTS.md, Integration: segment flagset).
 * From D10 (w19a) unchanged except the header field names. No shaping devices, no unused
 * locals, no pad arrays, no false prototypes. */
#include "boot_tail_audio_record.h"
extern unsigned int D_8003828C;
void func_80011A64(AudioState *audio, unsigned short samples, unsigned char *active)
{
    unsigned int increment;
    float ratio;
    increment = ((samples * 32000U) / D_8003828C) << 11;
    if (audio->pending == 0 && audio->state != 3) {
        audio->state = 3;
        audio->step = 0;
        audio->saved_value = audio->value;
        audio->scale = 0.0f;
        audio->count = audio->release_count;
    } else if ((audio->step >> 16) >= audio->count) {
        switch (audio->state) {
        case 0:
            audio->state = 1;
            audio->step = 0;
            audio->saved_value = 1.0f;
            audio->scale = audio->value24;
            audio->count = audio->value22;
            break;
        case 1:
            audio->state = 2;
            audio->saved_value = audio->value24;
            audio->scale = audio->value24;
            audio->value = audio->value24;
            return;
        case 3:
            *active = 0;
            audio->value = 0.0f;
            return;
        }
    }
    if (audio->count != 0) {
        ratio = audio->step / (audio->count * 65536.0f);
    } else {
        ratio = 1.0f;
    }
    audio->value = (audio->scale - audio->saved_value) * ratio + audio->saved_value;
    audio->step += increment;
}
