#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef CANDIDATE_PATH
#define CANDIDATE_PATH "../../matches/ovl_b/func_8038CA24.c"
#endif
#include CANDIDATE_PATH

PlayerHudState D_80152818[4];
s8 D_80399118[8];
HudGroup D_80399550[4];
HudSlots D_80399120[4];

typedef char check_player_size[sizeof(PlayerHudState) == 0x3b8 ? 1 : -1];
typedef char check_group_size[sizeof(HudGroup) == 0x148 ? 1 : -1];
typedef char check_slots_size[sizeof(HudSlots) == 0x10c ? 1 : -1];
typedef char check_float[sizeof(float) == 4 ? 1 : -1];

static unsigned int random_word(unsigned int *state)
{
    *state = *state * 1664525U + 1013904223U;
    return *state;
}

static void fill(void *buffer, size_t count, unsigned int *state)
{
    unsigned char *p = buffer;
    size_t i;
    for (i = 0; i < count; ++i) p[i] = (unsigned char)(random_word(state) >> 24);
}

static int run(unsigned int seed, int player)
{
    unsigned char car[sizeof(D_80152818)];
    unsigned char modes[sizeof(D_80399118)];
    unsigned char groups[sizeof(D_80399550)];
    unsigned char slots[sizeof(D_80399120)];
    unsigned char *p;
    int i;
    fill(D_80152818, sizeof(D_80152818), &seed);
    fill(D_80399118, sizeof(D_80399118), &seed);
    fill(D_80399550, sizeof(D_80399550), &seed);
    fill(D_80399120, sizeof(D_80399120), &seed);
    memcpy(car, D_80152818, sizeof(car));
    memcpy(modes, D_80399118, sizeof(modes));
    memcpy(groups, D_80399550, sizeof(groups));
    memcpy(slots, D_80399120, sizeof(slots));
    p = car + player * 0x3b8;
    memset(p + 0x38c, 0, 4);
    p[0x3a2] = 0;
    p[0x3a0] = 0;
    memset(p + 0x3a4, 0, 4);
    p[0x384] = 8;
    p[0x385] = 255;
    modes[player] = 9;
    p = groups + player * 0x148;
    memset(p + 0x104, 255, 4);
    p[0x138] = 255;
    memset(p + 0x144, 0, 4);
    p = slots + player * 0x10c;
    memset(p + 0x108, 0, 4);
    for (i = 0; i != 5; ++i) memset(p + i * 4, 255, 4);
    func_8038CA24((s16)player);
    if (memcmp(car, D_80152818, sizeof(car))) return 1;
    if (memcmp(modes, D_80399118, sizeof(modes))) return 2;
    if (memcmp(groups, D_80399550, sizeof(groups))) return 3;
    if (memcmp(slots, D_80399120, sizeof(slots))) return 4;
    return 0;
}

int main(void)
{
    unsigned int seed;
    int player, result;
    for (seed = 0; seed < 4096; ++seed) {
        for (player = 0; player < 4; ++player) {
            result = run(seed, player);
            if (result) {
                fprintf(stderr, "mismatch seed=%u player=%d array=%d\n", seed, player, result);
                return 1;
            }
        }
    }
    puts("16384 complete-state host cases passed");
    return 0;
}
