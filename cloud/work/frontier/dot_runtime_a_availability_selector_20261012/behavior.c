/* Bounded host fixture appended to the actual source's producer prefix.
 * External arrays are test storage. This is not native or gameplay execution. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

s16 D_8014A108;
s8 D_803BADBC[4], D_803BADC0[4], D_803BAD78[4];
s32 D_803BAC20[4], D_803BAC68[4], D_803BACE0[4];
s8 D_803BAD00[4][14], D_803BAD40[4][14];

static unsigned int random_state = 94564U;
static unsigned int next_random(void)
{
    random_state = random_state * 1664525U + 1013904223U;
    return random_state;
}

int main(void)
{
    int fixture, selected, p, j, first, last, level, mode, busy, enabled, total;
    unsigned int cases = 0;
    unsigned int maps = 0, states = 0, modes = 0, busy_values = 0;
    s8 primary[4][14], secondary[4][14], levels[4], counts[4];
    for (fixture = 0; fixture < 5000; fixture++) {
        D_8014A108 = fixture % 5;
        for (p = 0; p < 4; p++) {
            D_803BADBC[p] = (next_random() >> 16) % 4;
            D_803BAC20[p] = (int)((next_random() >> 16) % 14) - 4;
            D_803BAC68[p] = (next_random() >> 16) % 3;
            D_803BACE0[p] = (next_random() >> 16) % 6;
            maps |= 1U << D_803BADBC[p];
            states |= 1U << (D_803BAC20[p] + 4);
            modes |= 1U << D_803BACE0[p];
            busy_values |= 1U << D_803BAC68[p];
        }
        for (selected = -1; selected < 4; selected++) {
            memset(D_803BAD00, -77, sizeof(D_803BAD00));
            memset(D_803BAD40, -77, sizeof(D_803BAD40));
            memset(D_803BADC0, -77, sizeof(D_803BADC0));
            memset(D_803BAD78, -77, sizeof(D_803BAD78));
            memset(primary, -77, sizeof(primary));
            memset(secondary, -77, sizeof(secondary));
            memset(levels, -77, sizeof(levels));
            memset(counts, -77, sizeof(counts));
            first = selected == -1 ? 0 : selected;
            last = selected == -1 ? D_8014A108 : selected + 1;
            for (p = first; p < last; p++) {
                level = D_803BAC20[(int)D_803BADBC[p]];
                busy = D_803BAC68[(int)D_803BADBC[p]] != 0;
                mode = D_803BACE0[p];
                levels[p] = level;
                total = 0;
                for (j = 0; j < 14; j++) {
                    if (j < 2) enabled = mode != 4;
                    else if (j == 2 || j == 13) enabled = 0;
                    else if (j < 11) enabled = j - 3 < level;
                    else if (j == 11) enabled = mode != 4 && level >= 0 && !busy;
                    else enabled = mode != 4 && level > 0;
                    primary[p][j] = enabled;
                    if (j == 1 && mode == 4) enabled = 1;
                    if (j == 2) enabled = level < 1 || (busy && mode != 4);
                    secondary[p][j] = enabled;
                    total += enabled;
                }
                counts[p] = total;
            }
            func_80394564(selected);
            if (memcmp(primary, D_803BAD00, sizeof(primary)) ||
                memcmp(secondary, D_803BAD40, sizeof(secondary)) ||
                memcmp(levels, D_803BADC0, sizeof(levels)) ||
                memcmp(counts, D_803BAD78, sizeof(counts))) {
                fprintf(stderr, "fixture %d selector %d failed\n", fixture, selected);
                return 1;
            }
            cases++;
        }
    }
    if (maps != 15U || states != 16383U || modes != 63U || busy_values != 7U) {
        fprintf(stderr, "fixture did not cover all advertised input values\n");
        return 1;
    }
    printf("PASS: %u bounded actual-source availability cases\n", cases);
    return 0;
}
