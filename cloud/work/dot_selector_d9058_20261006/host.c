/* Test-only queue and selector hooks; never part of a native candidate TU. */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
signed short D_80151AD0, D_8014A10A;
int D_80113E8C[24 * 8];
unsigned int D_801174B4;
unsigned char D_801461D0[24];
static int stage, mode, selected, initial_height, initial_index;
extern void func_800D9058(void);
int osRecvMesg(void *queue, void *message, int block)
{
    assert(stage == 0 && queue == D_801461D0 && message == 0 && block == 1);
    stage = 1;
    if (mode) {
        D_80151AD0 = initial_index + 1;
        D_80113E8C[24 * initial_index] = initial_height ^ 0x12345678;
    }
    return -1;
}
int slot_state_setup(int selection)
{
    assert(stage == 1 && selection == 11);
    selected = selection;
    stage = 2;
    if (mode == 2) D_801174B4 |= 2U;
    return initial_height ^ (int)D_801174B4;
}
int osJamMesg(void *queue, void *message, int block)
{
    assert(stage == 2 && queue == D_801461D0 && message == 0 && block == 0);
    stage = 3;
    if (mode == 3) D_801174B4 = 0;
    return 7654321;
}
int run_case(int height, int index, unsigned int flags, int mutation)
{
    assert(index >= 0 && index < 7 && mutation >= 0 && mutation < 4);
    initial_index = index;
    initial_height = height;
    mode = mutation;
    stage = selected = 0;
    D_80151AD0 = index;
    D_80113E8C[index * 24] = height;
    D_801174B4 = flags;
    D_8014A10A = -1777;
    func_800D9058();
    assert(stage == 3 && selected == 11);
    return D_8014A10A;
}
#ifdef HOST_MAIN
int main(void)
{
    unsigned int h = 1, flags;
    int i, m, numerator, expected;
    for (i = 0; i < 10000; i++) {
        h = h * 1664525U + 1013904223U;
        flags = h ^ 0x19325147U;
        for (m = 0; m < 4; m++) {
            numerator = (int)(h - 24U);
            expected = (short)(numerator / 16);
            if (expected > 13 || ((m == 3 ? 0U : flags | (m == 2 ? 2U : 0U)) & 0x7C03FFFEU)) expected = 13;
            assert(run_case((int)h, i % 7, flags, m) == expected);
        }
    }
    puts("sanitized source cases: 40000");
    return 0;
}
#endif
