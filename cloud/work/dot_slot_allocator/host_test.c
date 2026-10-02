/* Standalone host behavioral test; not part of the IDO translation unit. */
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "func_80091B00.c"
Slot D_80142DD8[128];
static uint32_t state = 0x519A7E21u;
static uint32_t next_random(void) {
    state ^= state << 13; state ^= state >> 17; state ^= state << 5;
    return state;
}
static void check_case(void) {
    Slot expected[128];
    Slot *result;
    int i, first = -1;
    memcpy(expected, D_80142DD8, sizeof expected);
    for (i = 0; i < 128; ++i) {
        if (expected[i].active == 0) { first = i; break; }
    }
    if (first >= 0) { expected[first].active = 1; expected[first].index = -1; }
    result = func_80091B00();
    assert(result == (first >= 0 ? &D_80142DD8[first] : NULL));
    assert(memcmp(expected, D_80142DD8, sizeof expected) == 0);
}
int main(void) {
    int position, active, iteration, i;
    unsigned char *bytes = (unsigned char *)D_80142DD8;
    size_t j;
    assert(sizeof(Slot) == 24 && offsetof(Slot, active) == 3);
    assert(offsetof(Slot, index) == 0);
    /* Every first-free position, including exhausted, against every byte
       representation of a nonzero active flag; later entries stay free. */
    for (active = -128; active <= 127; ++active) {
        if (active == 0) continue;
        for (position = 0; position <= 128; ++position) {
            memset(D_80142DD8, 0xA5, sizeof D_80142DD8);
            for (i = 0; i < 128; ++i)
                D_80142DD8[i].active = (signed char)(i < position ? active : 0);
            check_case();
        }
    }
    for (iteration = 0; iteration < 100000; ++iteration) {
        for (j = 0; j < sizeof D_80142DD8; ++j) bytes[j] = (unsigned char)next_random();
        check_case();
    }
    /* Allocate a full pool repeatedly, verifying progression and exhaustion. */
    memset(D_80142DD8, 0, sizeof D_80142DD8);
    for (i = 0; i < 130; ++i) check_case();
    puts("PASS: 133025 cases; first-free selection, full state preservation, all active bytes, repeated exhaustion");
    return 0;
}
