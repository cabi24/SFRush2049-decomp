/* C89 behavioral checks for the bounded, correctly typed array domain. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#ifndef CANDIDATE_SOURCE
#define CANDIDATE_SOURCE "group.c"
#endif
#include CANDIDATE_SOURCE
CarPart64 D_80139320[6];
Slot68 D_8012E700[16];
typedef char key_size[(sizeof(CarPart64) == 64) ? 1 : -1];
typedef char slot_size[(sizeof(Slot68) == 68) ? 1 : -1];
typedef char model_offset[(offsetof(CarPart64, model) == 20) ? 1 : -1];
typedef char first_offset[(offsetof(Slot68, first) == 60) ? 1 : -1];
typedef char second_offset[(offsetof(Slot68, second) == 64) ? 1 : -1];
typedef char word_size[(sizeof(s32) == 4) ? 1 : -1];
typedef char half_size[(sizeof(s16) == 2) ? 1 : -1];
int main(void)
{
    unsigned int highs[5] = {0U, 0x12340000U, 0x7fff0000U, 0x80000000U, 0xffff0000U};
    unsigned char keys_before[sizeof(D_80139320)];
    unsigned char slots_expected[sizeof(D_8012E700)];
    int key, slot, h, mode, cases = 0;
    for (key = 0; key < 6; key++) {
        for (slot = 0; slot < 16; slot++) {
            for (h = 0; h < 5; h++) {
                for (mode = 0; mode < 6; mode++) {
                    unsigned int raw = highs[h] | (unsigned int)slot;
                    int inputs[2] = {0x10203040, 0x50607080};
                    int *first, *second;
                    int first_value, second_value;
                    memset(D_80139320, 0x91, sizeof(D_80139320));
                    memset(D_8012E700, 0x27, sizeof(D_8012E700));
                    memcpy(&D_80139320[key].model, &raw, sizeof(raw));
                    first = &inputs[0]; second = &inputs[1];
                    if (mode == 1) {first = &D_8012E700[slot].second; second = &D_8012E700[slot].first;}
                    if (mode == 2) {first = &D_8012E700[slot].first; second = first;}
                    if (mode == 3) {first = &D_80139320[key].model; second = &D_8012E700[slot].first;}
                    if (mode == 4) {first = &D_8012E700[slot].second; second = &D_80139320[key].model;}
                    if (mode == 5) {second = first;}
                    memcpy(keys_before, D_80139320, sizeof(keys_before));
                    memcpy(slots_expected, D_8012E700, sizeof(slots_expected));
                    first_value = *first;
                    second_value = (second == &D_8012E700[slot].first) ? first_value : *second;
                    memcpy(slots_expected + slot*68 + 60, &first_value, 4);
                    memcpy(slots_expected + slot*68 + 64, &second_value, 4);
                    func_80092BF4((s16)key, first, second);
                    assert(memcmp(keys_before, D_80139320, sizeof(keys_before)) == 0);
                    assert(memcmp(slots_expected, D_8012E700, sizeof(slots_expected)) == 0);
                    cases++;
                }
            }
        }
    }
    assert(cases == 2880);
    printf("typed_alias_cases=%d\n", cases);
    return 0;
}
