/* Actual retained source is included by the compiler before this harness. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

MasterFader D_8004F300[32];
static unsigned int calls, converted_input, cases;

void func_8001E930(u32 *value)
{
    ++calls;
    converted_input = *value;
    *value *= 256U;
}

static void check(unsigned int volume, unsigned int duration, unsigned int group,
                  unsigned int type_seed, int uniform)
{
    MasterFader expected[32];
    unsigned int i, j, state, t, kind, n, d, delta;
    unsigned char *bytes;
    int selected;
    state = volume + duration * 65537U + group * 33U + type_seed;
    bytes = (unsigned char *)D_8004F300;
    for (i = 0; i < sizeof(D_8004F300); ++i) {
        state = state * 1664525U + 1013904223U;
        bytes[i] = (unsigned char)(state >> 24);
    }
    for (i = 0; i < 32; ++i) {
        D_8004F300[i].type = (unsigned char)(uniform ? type_seed : (i + type_seed) % 8);
    }
    memcpy(expected, D_8004F300, sizeof(expected));
    d = (duration & 65535U) ? (duration & 65535U) : 1U;
    t = (volume & 255U) * 65536U;
    for (i = 0; i < 32; ++i) {
        kind = expected[i].type;
        selected = group < 32 ? i == group :
            group == 255 ? (kind == 0 || kind == 1) :
            group == 252 ? (kind == 2 || kind == 3) :
            group == 250 ? kind == 2 : group == 251 ? kind == 3 :
            group == 253 ? kind == 0 : kind == 1;
        if (selected) {
            n = t - expected[i].pauseVolume;
            delta = n & 0x80000000U ? 0U - ((0U - n) / d) : n / d;
            expected[i].pauseTarget = t;
            expected[i].pauseDelta = delta;
            expected[i].pauseTime = d * 256U;
        }
    }
    calls = 0;
    func_8001BE14((u8)volume, (u16)duration, (u8)group);
    assert(calls == 1 && converted_input == d);
    for (j = 0; j < sizeof(expected); ++j) {
        assert(((unsigned char *)expected)[j] == ((unsigned char *)D_8004F300)[j]);
    }
    ++cases;
}

int main(void)
{
    unsigned int g, t, v, k, group;
    unsigned int times[5] = {0, 1, 2, 257, 65535};
    unsigned int volumes[4] = {0, 1, 127, 255};
    assert(sizeof(MasterFader) == 40);
    assert(offsetof(MasterFader, type) == 20);
    assert(offsetof(MasterFader, pauseVolume) == 24);
    assert(offsetof(MasterFader, pauseTarget) == 28);
    assert(offsetof(MasterFader, pauseDelta) == 32);
    assert(offsetof(MasterFader, pauseTime) == 36);
    for (g = 0; g < 38; ++g) {
        group = g < 32 ? g : g + 218;
        for (t = 0; t < 5; ++t) {
            for (v = 0; v < 4; ++v) {
                check(volumes[v], times[t], group, g, 0);
            }
        }
        for (k = 0; k < 256; ++k) {
            check(k, 17, group, k, 1);
        }
    }
    for (t = 0; t < 65536; ++t) {
        g = t % 38;
        check((t * 13U) & 255U, t, g < 32 ? g : g + 218, t % 8, 0);
    }
    printf("{\"result\":\"PASS\",\"calls\":%u}\n", cases);
    return 0;
}
