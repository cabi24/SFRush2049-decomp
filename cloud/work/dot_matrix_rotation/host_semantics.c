/* Standalone semantic tests; mocks are not part of the IDO candidate. */
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static float mock_sin(float);
static float mock_cos(float);
#define sinf mock_sin
#define cosf mock_cos
#include "func_800B5898.c"
#undef sinf
#undef cosf

float D_80123DB4, D_80123DB8;
static float angle_expected, sine_value, cosine_value;
static unsigned calls;
static int equal_float(float a, float b)
{
    uint32_t aa, bb;
    if (isnan(a) && isnan(b)) return 1;
    memcpy(&aa, &a, sizeof aa);
    memcpy(&bb, &b, sizeof bb);
    return aa == bb;
}
static float mock_sin(float angle)
{
    assert(calls++ == 0);
    assert(equal_float(angle, angle_expected));
    return sine_value;
}
static float mock_cos(float angle)
{
    assert(calls++ == 1);
    assert(equal_float(angle, angle_expected));
    return cosine_value;
}
int main(void)
{
    static const float angles[] = {
        -INFINITY, -1000.0f, -1.0f, -0.011f, -0.01f, -0.009f,
        -0.0f, 0.0f, 0.009f, 0.01f, 0.011f, 1.0f, 1000.0f,
        INFINITY, NAN
    };
    static const float bounds[][2] = {
        {-0.01f, 0.01f}, {0.0f, 0.0f}, {1.0f, -1.0f},
        {-INFINITY, INFINITY}, {NAN, 0.01f}, {-0.01f, NAN}, {NAN, NAN}
    };
    static const float special[] = {-0.0f, 0.0f, 1.0f, -1.0f, INFINITY, -INFINITY, NAN};
    uint32_t seed = 0x12345678U;
    unsigned b, a, trial, i, row, count = 0;
    for (b = 0; b < sizeof bounds / sizeof bounds[0]; ++b) {
        D_80123DB4 = bounds[b][0]; D_80123DB8 = bounds[b][1];
        for (a = 0; a < sizeof angles / sizeof angles[0]; ++a) {
            for (trial = 0; trial < 1024; ++trial) {
                float actual[11], expected[11];
                int active;
                for (i = 0; i < 11; ++i) {
                    seed = seed * 1664525U + 1013904223U;
                    actual[i] = ((int)(seed % 2000001U) - 1000000) / 37.0f;
                    if (trial < 7) actual[i] = special[(trial + i) % 7];
                }
                memcpy(expected, actual, sizeof actual);
                angle_expected = angles[a];
                sine_value = (trial % 201 - 100.0f) / 100.0f;
                cosine_value = (trial % 197 - 98.0f) / 98.0f;
                if (trial < 7) {sine_value = special[trial]; cosine_value = special[6-trial];}
                active = angle_expected < D_80123DB4 || D_80123DB8 < angle_expected;
                if (active) {
                    for (row = 0; row < 3; ++row) {
                        float y = expected[1 + row * 3 + 1];
                        float z = expected[1 + row * 3 + 2];
                        float product = y * cosine_value;
                        expected[1 + row * 3 + 1] = product - z * sine_value;
                        expected[1 + row * 3 + 2] = y * sine_value + z * cosine_value;
                    }
                }
                calls = 0;
                func_800B5898(angle_expected, actual + 1);
                assert(calls == (active ? 2U : 0U));
                for (i = 0; i < 11; ++i) assert(equal_float(actual[i], expected[i]));
                ++count;
            }
        }
    }
    printf("PASS: %u matrix/deadband cases; call order, arguments and canaries checked\n", count);
    return 0;
}
