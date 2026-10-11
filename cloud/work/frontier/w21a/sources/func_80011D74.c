/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* func_80011D74 - lib_11640 (IDO 7.1). Advance one native audio voice's sample position
 * (AudioState.sample_position, a double) by rate * samples / 4096: a looping voice (loop_length
 * != 0) that is still held (pending) or has no tail, and whose floor(position) (func_8001E688,
 * lib_1ece0) has not passed loop_start + loop_length, wraps by the loop length (the remainder
 * after floor(overshoot / loop_length) whole loops); otherwise the position advances and is
 * clamped to length - 1 at the end of the sample.
 * Field names are the header's (pending 0x01, sample_position 0x08, tail 0x3C); no private view.
 * FLAGS: the match needs as1's -r4300_mul (-Wab,-r4300_mul), which the Makefile passes to every
 * ROM TU (CFLAGS) and context_check.py uses; it stops as1 putting the increment's mul.d in the
 * `beqz loop_length` delay slot, so retail's `beqzl` takes the else path's `lw length` instead
 * (true=420 without it: the w19a/w20a "schedule only" residual). The lock flagset therefore
 * carries -Wab,-r4300_mul (see w21a/RESULTS.md, Integration: segment flagset).
 * From D10 (w19a) unchanged except the header field names. No shaping devices, no unused
 * locals, no pad arrays, no false prototypes. */
#include "boot_tail_audio_record.h"
extern double func_8001E688(double);
void func_80011D74(AudioState *audio, unsigned short samples)
{
    double increment;
    double loop_end;
    increment = (audio->rate * samples) * 0.000244140625;
    if (audio->loop_length != 0 && (audio->pending != 0 || audio->tail == 0)
        && func_8001E688(audio->sample_position) <= audio->loop_start + audio->loop_length) {
        loop_end = audio->loop_start + audio->loop_length;
        if (audio->sample_position + increment < loop_end) {
            audio->sample_position += increment;
        } else {
            increment -= loop_end - audio->sample_position;
            increment -= func_8001E688(increment / audio->loop_length) * audio->loop_length;
            audio->sample_position = audio->loop_start + increment;
        }
    } else {
        if (audio->sample_position + increment >= audio->length) {
            audio->sample_position = audio->length - 1;
        } else {
            audio->sample_position += increment;
        }
    }
}
