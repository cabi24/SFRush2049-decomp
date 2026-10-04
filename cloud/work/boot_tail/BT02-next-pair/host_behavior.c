#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "host_types.h"
extern void func_80011A64(AudioState *, unsigned short, unsigned char *);
extern void func_80011D74(AudioState *, unsigned short);
unsigned int D_8003828C;
static unsigned int calls;
static double arguments[2];
double func_8001E688(double value)
{
    assert(calls < 2);
    assert(value >= 0.0 && value < 4294967296.0);
    arguments[calls++] = value;
    return (double)(unsigned int)value;
}
static void check_envelope(void)
{
    static const unsigned int rates[] = { 1U, 32000U, 44100U, 0xffffffffU };
    static const unsigned short samples[] = { 0, 1, 192, 65535 };
    static const unsigned int steps[] = { 0U, 65535U, 65536U, 196608U, 0xffffffffU };
    AudioState a, e;
    unsigned char flag, expected_flag;
    unsigned int state, held, count, r, s, t, inc;
    float ratio;
    for (state = 0; state < 5; state++)
    for (held = 0; held < 2; held++)
    for (count = 0; count < 2; count++)
    for (r = 0; r < 4; r++)
    for (s = 0; s < 4; s++)
    for (t = 0; t < 5; t++) {
        memset(&a, 0xA5, sizeof(a));
        a.held = (unsigned char)held;
        a.state = (unsigned char)state;
        a.step = steps[t];
        a.count = count ? 3 : 0;
        a.release_count = 7;
        a.decay_count = 9;
        a.value = 0.625f;
        a.saved_value = 0.75f;
        a.scale = 0.125f;
        a.sustain = 0.25f;
        e = a;
        flag = expected_flag = 0x7B;
        D_8003828C = rates[r];
        inc = ((samples[s] * 32000U) / D_8003828C) << 11;
        if (held == 0 && state != 3) {
            e.state = 3; e.step = 0; e.saved_value = e.value;
            e.scale = 0.0f; e.count = e.release_count;
        } else if ((e.step >> 16) >= e.count) {
            if (state == 0) {
                e.state = 1; e.step = 0; e.saved_value = 1.0f;
                e.scale = e.sustain; e.count = e.decay_count;
            } else if (state == 1) {
                e.state = 2; e.saved_value = e.sustain;
                e.scale = e.sustain; e.value = e.sustain;
                func_80011A64(&a, samples[s], &flag);
                assert(memcmp(&a, &e, sizeof(a)) == 0);
                assert(flag == expected_flag);
                continue;
            } else if (state == 3) {
                expected_flag = 0; e.value = 0.0f;
                func_80011A64(&a, samples[s], &flag);
                assert(memcmp(&a, &e, sizeof(a)) == 0);
                assert(flag == expected_flag);
                continue;
            }
        }
        ratio = e.count ? e.step / (e.count * 65536.0f) : 1.0f;
        e.value = (e.scale - e.saved_value) * ratio + e.saved_value;
        e.step += inc;
        func_80011A64(&a, samples[s], &flag);
        assert(memcmp(&a, &e, sizeof(a)) == 0);
        assert(flag == expected_flag);
    }
}
static void check_position_case(unsigned int held, unsigned int stop,
                                unsigned int start, unsigned int loop,
                                unsigned int length, double position,
                                unsigned short rate, unsigned short samples)
{
    AudioState a, e;
    unsigned int expected_calls;
    double expected_arguments[2], increment, end, expected;
    memset(&a, 0x5A, sizeof(a));
    a.held = (unsigned char)held; a.stop_loop = stop;
    a.loop_start = start; a.loop_length = loop; a.length = length;
    a.position = position; a.rate = rate; e = a;
    expected_calls = 0;
    increment = (rate * samples) / 4096.0;
    end = (double)(start + loop);
    expected = position + increment;
    if (loop && (held || !stop)) {
        expected_arguments[expected_calls++] = position;
        if ((double)(unsigned int)position <= end) {
            if (!(expected < end)) {
                increment -= end - position;
                expected_arguments[expected_calls++] = increment / loop;
                expected = start + increment - (double)(unsigned int)(increment / loop) * loop;
            }
        } else if (expected >= length) {
            expected = (double)(length - 1U);
        }
    } else if (expected >= length) {
        expected = (double)(length - 1U);
    }
    e.position = expected;
    calls = 0;
    func_80011D74(&a, samples);
    assert(a.position == e.position);
    assert(memcmp(&a, &e, sizeof(a)) == 0);
    assert(calls == expected_calls);
    if (calls) assert(arguments[0] == expected_arguments[0]);
    if (calls == 2) assert(arguments[1] == expected_arguments[1]);
}
int main(void)
{
    unsigned int held, stop;
    assert(sizeof(AudioState) == 104);
    assert(offsetof(AudioState, rate) == 4);
    assert(offsetof(AudioState, position) == 8);
    assert(offsetof(AudioState, decay_count) == 0x22);
    assert(offsetof(AudioState, sustain) == 0x24);
    assert(offsetof(AudioState, length) == 0x30);
    assert(offsetof(AudioState, loop_start) == 0x34);
    assert(offsetof(AudioState, loop_length) == 0x38);
    assert(offsetof(AudioState, stop_loop) == 0x3C);
    assert(offsetof(AudioState, count) == 0x48);
    assert(offsetof(AudioState, value) == 0x4C);
    assert(offsetof(AudioState, state) == 0x5C);
    check_envelope();
    for (held = 0; held < 2; held++)
    for (stop = 0; stop < 2; stop++) {
        check_position_case(held, stop, 10, 20, 100, 12.25, 4096, 1);
        check_position_case(held, stop, 10, 20, 100, 29.0, 4096, 1);
        check_position_case(held, stop, 10, 20, 100, 29.75, 4096, 43);
        check_position_case(held, stop, 10, 20, 100, 30.75, 4096, 2);
        check_position_case(held, stop, 10, 20, 100, 31.0, 4096, 1);
        check_position_case(held, stop, 10, 20, 100, 99.75, 4096, 1);
        check_position_case(held, stop, 10, 0, 100, 90.0, 4096, 10);
        check_position_case(held, stop, 0, 0, 0, 0.0, 0, 65535);
        check_position_case(held, stop, 10, 20, 100, 12.25, 65535, 192);
        check_position_case(held, stop, 10, 20, 100, 12.25, 0, 0);
        check_position_case(held, stop, 0x80000000U, 20, 0xffffffffU,
                            2147483649.0, 4096, 21);
        check_position_case(held, stop, 0, 0, 0xffffffffU, 4294967294.0, 4096, 2);
    }
    puts("PASS: 1600 envelope cases, 48 position cases, field layout and untouched bytes");
    return 0;
}
