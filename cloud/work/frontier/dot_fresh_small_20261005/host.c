/* Bounded host semantic checks; this is not a native-instruction emulator. */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "func_800DC628.c"
#include "func_800D4DFC.c"

s32 D_801170F8;
u8 permutation[32];
u8 *D_80116FE4 = permutation;
u8 D_80116FE8[32], D_8012E618[32];
u16 D_801170E8, D_801170EC, D_801170F0, D_801170F4;
static u32 rng = 0xD4DFC;
static void *expected_object;
static int basis_calls;

static u32 next_random(void) {
    rng = rng * 1664525U + 1013904223U;
    return rng;
}

void math_utility(f32 *source, f32 *dest) {
    int i;
    assert((u8 *)source == (u8 *)expected_object + 748);
    assert((u8 *)dest == (u8 *)expected_object + 1952);
    basis_calls++;
    for (i = 0; i < 9; i++) dest[i] = source[i];
}

static void packing_case(u32 first, u32 second, int initialize) {
    u8 inverse[32], buffer[32];
    u16 length, old_first = 0x4142, old_second = 0x4344, old_position = 0x4546;
    unsigned i;
    int expected, actual;
    for (i = 0; i < 32; i++) permutation[i] = (u8)i;
    for (i = 31; i; i--) {
        unsigned j = next_random() % (i + 1);
        u8 temp = permutation[i]; permutation[i] = permutation[j]; permutation[j] = temp;
    }
    for (i = 0; i < 32; i++) inverse[i] = D_80116FE8[i] = (u8)next_random();
    memset(D_8012E618, 0xA5, 32);
    memset(buffer, 0xA5, 32);
    if (initialize) for (i = 0; i < 32; i++) inverse[permutation[i]] = (u8)i;
    D_801170F8 = initialize;
    D_801170E8 = 0xBEEF;
    D_801170EC = old_first; D_801170F0 = old_second; D_801170F4 = old_position;
    length = (u16)((first + second + 7U) >> 3);
    expected = length <= 32 && second <= 32;
    if (expected) memset(buffer, 0, length);
    actual = func_800DC628(first, second);
    assert(actual == expected && D_801170F8 == 0);
    assert(!memcmp(D_80116FE8, inverse, 32));
    assert(!memcmp(D_8012E618, buffer, 32));
    assert(D_801170E8 == length);
    assert(D_801170EC == (expected ? (u16)first : old_first));
    assert(D_801170F0 == (expected ? (u16)second : old_second));
    assert(D_801170F4 == (expected ? 0 : old_position));
}

static void snapshot_case(void) {
    union { double alignment; u8 bytes[1988]; } object, expected;
    static const unsigned scalar_offsets[] = {1256, 1328, 1348, 1420};
    static const unsigned copy_groups[][3] = {{748,1952,9},{1484,1884,4},
                                             {1516,1900,4},{1844,1916,9}};
    unsigned i, j;
    f32 a, b, sum, amount;
    s16 narrowed;
    memset(object.bytes, 0xA5, sizeof(object.bytes));
    for (i = 0; i < 4; i++) {
        f32 value = ((int)(next_random() % 2049) - 1024) / 128.0f;
        memcpy(object.bytes + scalar_offsets[i], &value, 4);
    }
    for (i = 0; i < 4; i++) for (j = 0; j < copy_groups[i][2]; j++) {
        f32 value = ((int)(next_random() % 8193) - 4096) / 64.0f;
        memcpy(object.bytes + copy_groups[i][0] + j * 4, &value, 4);
    }
    memcpy(expected.bytes, object.bytes, sizeof(object.bytes));
    a = FLOAT_AT(object.bytes, 1348) * FLOAT_AT(object.bytes, 1420);
    b = FLOAT_AT(object.bytes, 1328) * FLOAT_AT(object.bytes, 1256);
    sum = a + b;
    amount = (sum * 0.5f) * 2.72727275f;
    narrowed = (s16)(s32)amount;
    memcpy(expected.bytes + 1880, &narrowed, 2);
    for (i = 0; i < 4; i++)
        memcpy(expected.bytes + copy_groups[i][1], object.bytes + copy_groups[i][0], copy_groups[i][2] * 4);
    expected_object = object.bytes;
    basis_calls = 0;
    func_800D4DFC(object.bytes);
    assert(basis_calls == 1);
    assert(!memcmp(object.bytes, expected.bytes, sizeof(object.bytes)));
}

int main(void) {
    static const u32 edges[] = {0,1,7,8,31,32,33,248,249,255,256,257,65535,65536,0xFFFFFFF8U,0xFFFFFFFFU};
    unsigned i, j, init;
    unsigned packing_cases = 0;
    for (init = 0; init < 2; init++) for (i = 0; i < sizeof(edges)/sizeof(edges[0]); i++)
        for (j = 0; j < sizeof(edges)/sizeof(edges[0]); j++) {
            packing_case(edges[i], edges[j], init); packing_cases++;
        }
    for (i = 0; i < 4096; i++) {
        u32 first = i & 1 ? next_random() : next_random() % 260;
        u32 second = next_random() % 40;
        packing_case(first, second, i & 1); packing_cases++;
        snapshot_case();
    }
    printf("packing_cases=%u snapshot_cases=4096\n", packing_cases);
    return 0;
}
