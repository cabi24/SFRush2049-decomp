#include <stddef.h>
#include <stdint.h>
#include <string.h>
#ifndef CANDIDATE
#define CANDIDATE "candidate.c"
#endif
#include CANDIDATE
s32 D_80143FF4, D_8014A110;
f32 D_8002AFB8, D_801543CC;
ModelClockRecord D_8014A250[6];
typedef char assert_stride[(sizeof(ModelClockRecord) == 2056) ? 1 : -1];
typedef char assert_ticks[(offsetof(ModelClockRecord, ticks) == 1808) ? 1 : -1];
typedef char assert_time[(offsetof(ModelClockRecord, last_time) == 1812) ? 1 : -1];
typedef char assert_step[(offsetof(ModelClockRecord, step) == 1816) ? 1 : -1];
void run_case(const uint32_t *input, uint32_t *output)
{
    int i;
    memset(D_8014A250, 0xA5, sizeof(D_8014A250));
    memcpy(&D_8014A110, input + 1, 4);
    memcpy(&D_8002AFB8, input + 2, 4);
    D_80143FF4 = 123;
    D_801543CC = 567.0f;
    for (i = 0; i < 6; i++)
        memcpy(&D_8014A250[i].step, input + 3 + i, 4);
    func_800E762C(input[0]);
    memcpy(output, &D_80143FF4, 4);
    memcpy(output + 1, &D_801543CC, 4);
    memcpy(output + 2, D_8014A250, sizeof(D_8014A250));
}
