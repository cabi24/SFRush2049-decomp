/* Appended to the unchanged candidate by review.py. C89 host behavior only.
 * Pointer-valued service tokens are compared, never dereferenced.
 */
#include <string.h>
typedef unsigned int u32;
typedef char host_pointer_carrier[(sizeof(void *) <= sizeof(unsigned long)) ? 1 : -1];
u8 D_80151960;
s32 D_8011EAA0, D_80151A6C, D_80151AD4;
void *D_80151ADC;
SeqEntry D_8011F070[65538];
ResourceGroup *D_80152464;
void *D_801525FC;
SampleRecord *D_80152690;
ResourceOffsets *D_801526D8;
static u32 test[12];
static u8 active[8];
static int phase, failed;
#define CHECK(c) do { if (!(c)) failed = 1; } while (0)
#define TOKEN(v) ((void *)(unsigned long)(v))

s32 audio_frame_sync(s32 a, s32 b, s32 c, s32 d, void *e)
{
    CHECK(phase++ == 0);
    CHECK((u32)a == test[2] + 10 && b == 0 && c == 0 && d == 0 && e == 0);
    return test[6];
}

void display_list_alloc(s32 index)
{
    CHECK(phase++ == 1);
    CHECK((u32)index == test[6]);
    if (test[8]) D_80151AD4 = test[7];
    active[index] = 1;
}

void *slot_value_get(s32 index)
{
    CHECK(phase++ == 2);
    CHECK((u32)index == (test[8] ? test[7] : test[6]));
    return TOKEN(0x60700000U + index * 0x1000U);
}

s32 func_8001536C(ResourceGroup *groups, u16 id, void *address,
                  SampleRecord *records, ResourceOffsets *resources)
{
    CHECK(phase++ == 3);
    CHECK(groups == D_80152464 && id == test[5] && address == D_801525FC);
    CHECK(records == D_80152690 && resources == D_801526D8);
    if (test[9]) {
        D_8011F070[test[2]].h2 ^= 0xFFFF;
        D_80151ADC = TOKEN(0x60908000U);
    }
    return test[10];
}

s32 func_800156E8(u16 id, u16 program, void *stream, SequenceOptions *options)
{
    u32 handle;
    CHECK(phase++ == 4);
    handle = test[8] ? test[7] : test[6];
    CHECK(id == (test[9] ? (test[5] ^ 0xFFFF) : test[5]));
    CHECK(program == (test[2] & 0xFFFF));
    CHECK(stream == TOKEN(test[9] ? 0x60908000U : 0x60700000U + handle * 0x1000U));
    CHECK(options == 0);
    return test[11];
}

int independent_host_case(const u32 *input)
{
    int run, i;
    u32 handle;
    memcpy(test, input, sizeof(test));
    phase = failed = 0;
    for (i = 0; i < 8; ++i) active[i] = i * 13 + 2;
    D_80151960 = test[0];
    D_8011EAA0 = test[1];
    D_80151A6C = test[3];
    D_80151AD4 = 7;
    D_80151ADC = TOKEN(0x60601000U);
    D_80152464 = TOKEN(0x60602000U);
    D_801525FC = TOKEN(0x60603000U);
    D_80152690 = TOKEN(0x60604000U);
    D_801526D8 = TOKEN(0x60605000U);
    D_8011F070[test[2]].h0 = 0x8BCD;
    D_8011F070[test[2]].h2 = test[5];
    run = !test[0] && (test[1] == 0xFFFFFFFFU || test[2] != test[3]);
    func_800979A0(test[2], test[4]);
    CHECK(phase == (run ? 5 : 0));
    CHECK((u32)D_8011EAA0 == (run ? test[11] : test[1]));
    CHECK((u32)D_80151A6C == (run ? test[2] : test[3]));
    handle = test[8] ? test[7] : test[6];
    CHECK((u32)D_80151AD4 == (run ? handle : 7));
    CHECK(D_80151ADC == TOKEN(run ? (test[9] ? 0x60908000U : 0x60700000U + handle * 0x1000U) : 0x60601000U));
    CHECK(D_8011F070[test[2]].h0 == 0x8BCD);
    CHECK(D_8011F070[test[2]].h2 == ((run && test[9]) ? (test[5] ^ 0xFFFF) : test[5]));
    for (i = 0; i < 8; ++i) CHECK(active[i] == ((run && (u32)i == test[6]) ? 1 : i * 13 + 2));
    return failed;
}
