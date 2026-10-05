/* Initializer contract test. The two callees log events, not rendering behavior. */
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "init.c"

Gfx *D_80149438;
Gfx D_80124FE8[2][2400];
u32 D_8012E608;
s32 D_8012E60C, D_8012E668, D_8012E610;
s32 D_8002AFC0, D_8002AFC4, D_8012E674;
s16 D_8012E67A;
u8 D_8011EACF;
void *D_8012E684;
s32 D_8012E6C0, D_8014A248, D_8012E680, D_8012E6D0;
s64 D_8012E688;

static int mode_calls, flag_calls, next_height, change_height;
static int expected_bank, expected_width, expected_height;
static unsigned cases;

static void check_reset(void)
{
    assert(D_8012E608 == 0 && D_8012E60C == 0 && D_8012E668 == 0);
    assert(D_8012E610 == expected_width - 1);
    assert(D_8012E674 == expected_height - 1);
    assert(D_8012E67A == 0 && D_8011EACF == 255);
    assert(D_8012E684 == 0 && D_8012E688 == 0);
    assert(D_8012E6C0 == expected_bank);
    assert(D_8014A248 == -1 && D_8012E680 == -1 && D_8012E6D0 == 0);
}

void func_80086A50(s32 mode)
{
    assert(mode == 1 && mode_calls == 0 && flag_calls == 0);
    check_reset();
    assert(D_80149438 == D_80124FE8[expected_bank] + 4);
    ++mode_calls;
    if (change_height) D_8002AFC4 = next_height;
}

void func_800878E0(u32 flags)
{
    assert(flags == 0x8000 && flag_calls == 0 && mode_calls == 1);
    assert(D_8002AFC4 >= 221);
    ++flag_calls;
}

static void one_case(s32 bank, s32 width, s32 height, int changed, s32 after)
{
    int i, b;
    static const u32 commands[4][2] = {
        {0xF9000000U, 16}, {0xE7000000U, 0},
        {0xE3000C00U, 0}, {0xD9000000U, 0}
    };
    expected_bank = bank + 1 >= 2 ? 0 : bank + 1;
    expected_width = width;
    expected_height = height;
    change_height = changed;
    next_height = after;
    D_80149438 = 0;
    D_8002AFC0 = width;
    D_8002AFC4 = height;
    D_8012E6C0 = bank;
    D_8012E608 = 0x87654321U;
    D_8012E60C = D_8012E668 = D_8012E610 = D_8012E674 = 456;
    D_8012E67A = 123;
    D_8011EACF = 17;
    D_8012E684 = &D_8012E608;
    D_8012E688 = -123456789012345LL;
    D_8014A248 = D_8012E680 = D_8012E6D0 = 123;
    memset(D_80124FE8, 0xAB, sizeof(D_80124FE8));
    mode_calls = flag_calls = 0;
    sound_init();
    check_reset();
    assert(mode_calls == 1);
    assert(flag_calls == ((changed ? after : height) >= 221));
    assert(D_80149438 == D_80124FE8[expected_bank] + 4);
    assert(D_8002AFC0 == width);
    assert(D_8002AFC4 == (changed ? after : height));
    for (b = 0; b < 2; ++b) {
        for (i = 0; i < 2400; ++i) {
            u32 a = b == expected_bank && i < 4 ? commands[i][0] : 0xABABABABU;
            u32 z = b == expected_bank && i < 4 ? commands[i][1] : 0xABABABABU;
            assert(D_80124FE8[b][i].words.w0 == a);
            assert(D_80124FE8[b][i].words.w1 == z);
        }
    }
    ++cases;
}

struct State {
    Gfx *cursor;
    u32 flags;
    s32 left, top, right, bottom, width, height, bank, mode, tlut, auxiliary;
    s16 short_cache;
    u8 alpha;
    void *image;
    s64 key;
    Gfx commands[2][2400];
};

static void snapshot(struct State *state)
{
    memset(state, 0, sizeof(*state));
    state->cursor = D_80149438;
    state->flags = D_8012E608;
    state->left = D_8012E60C; state->top = D_8012E668;
    state->right = D_8012E610; state->bottom = D_8012E674;
    state->width = D_8002AFC0; state->height = D_8002AFC4;
    state->bank = D_8012E6C0; state->mode = D_8014A248;
    state->tlut = D_8012E680; state->auxiliary = D_8012E6D0;
    state->short_cache = D_8012E67A; state->alpha = D_8011EACF;
    state->image = D_8012E684; state->key = D_8012E688;
    memcpy(state->commands, D_80124FE8, sizeof(state->commands));
}

static void nonnull_noop(void)
{
    struct State before, after;
    D_80149438 = D_80124FE8[1] + 1700;
    D_8012E608 = 0xABCDEF01U;
    D_8012E60C = 7; D_8012E668 = 11; D_8012E610 = 13; D_8012E674 = 17;
    D_8002AFC0 = INT_MIN; D_8002AFC4 = INT_MIN; D_8012E6C0 = INT_MAX;
    D_8014A248 = 19; D_8012E680 = 23; D_8012E6D0 = 29;
    D_8012E67A = 31; D_8011EACF = 37;
    D_8012E684 = &D_8012E608; D_8012E688 = -123456789012345LL;
    mode_calls = flag_calls = 0;
    snapshot(&before);
    sound_init();
    snapshot(&after);
    assert(memcmp(&before, &after, sizeof(before)) == 0);
    assert(mode_calls == 0 && flag_calls == 0);
}

int main(void)
{
    static const s32 banks[] = {-1, 0, 1, 2, INT_MAX - 1};
    static const s32 widths[] = {0, 1, 320, INT_MAX};
    static const s32 heights[] = {-1, 0, 1, 220, 221, 480, INT_MAX};
    unsigned b, w, h, i;
    u32 random = 0x20492049U;
    assert(sizeof(u32) == 4 && sizeof(s64) == 8 && sizeof(Gfx) == 8);
    for (b = 0; b < sizeof(banks)/sizeof(banks[0]); ++b)
        for (w = 0; w < sizeof(widths)/sizeof(widths[0]); ++w)
            for (h = 0; h < sizeof(heights)/sizeof(heights[0]); ++h)
                one_case(banks[b], widths[w], heights[h], 0, 0);
    one_case(0, 320, 220, 1, 221);
    one_case(1, 640, 221, 1, 220);
    for (i = 0; i < 5000; ++i) {
        random = random * 1664525U + 1013904223U;
        one_case(random & 1, (random >> 1) % 1280 + 1,
                 (random >> 12) % 1000 + 1, 0, 0);
    }
    nonnull_noop();
    printf("graphics initializer: %u initialization cases and full-state no-op passed\n", cases);
    return 0;
}
