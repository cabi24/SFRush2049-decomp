/* Host-only doubles and tests. The four actual sources are separate units. */
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
u8 D_8002C630;
volatile u8 D_8004BE94;
u8 D_8004FA18, D_8004FA19, D_8004FA1A;
u32 D_800382A0, D_800382A4;
void *D_80038228[65536];
extern int func_800107E0(u32);
extern int func_80010840(u32, u8, u8, u8, u16, u32);
extern int func_800108E0(u32, u8, u8, u8, u16, u32);
extern int func_80013D70(u16, u16);
static int events[16], event_count, init_flag, reset_flag, setup_result;
static u32 expected_frequency, adjusted_frequency, expected_flags;
static u16 expected_count;
static u8 expected_voices, expected_setting1, expected_setting2;
static void *expected_buffer;
static int token[3];
static unsigned long cases;

static void log_event(int value)
{
    assert(event_count < 16);
    events[event_count++] = value;
}
static void init_event(int value)
{
    assert(D_8002C630 == init_flag);
    assert(D_8004BE94 == (value <= 3 ? reset_flag : 0));
    log_event(value);
}
void func_80014E10(void) { init_event(1); }
void func_80017108(void) { init_event(2); }
void func_800199F4(void) { init_event(3); }
void func_8001C1D8(u32 frequency)
{
    assert(frequency == expected_frequency);
    init_event(4);
}
void func_8001C390(void) { init_event(5); }
void func_8001E0C0(void) { init_event(6); }
static int setup(int kind, u32 *frequency, u16 voices, u16 count, u32 flags)
{
    assert(D_8002C630 == 0);
    assert(*frequency == expected_frequency);
    assert(voices == expected_voices && count == expected_count);
    assert(flags == expected_flags);
    assert(D_8004FA18 == expected_voices);
    assert(D_8004FA19 == expected_setting1);
    assert(D_8004FA1A == expected_setting2);
    assert(D_8004BE94 == reset_flag);
    log_event(kind);
    *frequency = adjusted_frequency;
    expected_frequency = adjusted_frequency;
    return setup_result;
}
int func_800143C0(u32 *frequency, u16 voices, u16 count, u32 flags)
{
    return setup(10, frequency, voices, count, flags);
}
int func_80014434(u32 *frequency, u16 voices, u16 count, u32 flags)
{
    return setup(11, frequency, voices, count, flags);
}
void func_800198C8(void)
{
    assert(D_800382A0 == D_800382A4);
    log_event(20);
}
void func_8001B154(void)
{
    assert(event_count == 1 && events[0] == 20);
    log_event(21);
}
void func_800139D4(void *buffer, u16 count)
{
    assert(buffer == expected_buffer && count == expected_count);
    log_event(22);
}
static void clear_events(void)
{
    memset(events, 0, sizeof(events));
    event_count = 0;
}
static void check_init(int offset)
{
    int i;
    assert(event_count == offset + 6);
    for (i = 0; i < 6; i++) { assert(events[offset + i] == i + 1); }
    assert(D_8004BE94 == 0 && D_8002C630 == 1);
}
static void test_initialize(void)
{
    static const u32 frequencies[] = {0, 1, 32000, UINT_MAX};
    int i, flag;
    for (i = 0; i < 4; i++) {
        for (flag = 0; flag < 3; flag++) {
            clear_events();
            init_flag = flag == 2 ? 255 : flag;
            reset_flag = flag == 2 ? 255 : 73;
            D_8002C630 = (u8)init_flag;
            D_8004BE94 = (u8)reset_flag;
            expected_frequency = frequencies[i];
            D_8004FA18 = 17; D_8004FA19 = 23; D_8004FA1A = 29;
            assert(func_800107E0(expected_frequency) == 0);
            check_init(0);
            assert(D_8004FA18 == 17 && D_8004FA19 == 23 && D_8004FA1A == 29);
            cases++;
        }
    }
}
static void test_wrappers(void)
{
    static const int results[] = {0, -1, 7, INT_MIN};
    static const u32 frequencies[] = {0, 1, 32000, UINT_MAX};
    unsigned int voice;
    int kind, r, actual;
    for (kind = 0; kind < 2; kind++) {
        for (voice = 0; voice < 256; voice++) {
            for (r = 0; r < 4; r++) {
                clear_events();
                init_flag = 0; reset_flag = 211;
                D_8002C630 = 255; D_8004BE94 = (u8)reset_flag;
                expected_frequency = frequencies[r];
                adjusted_frequency = expected_frequency ^ 0x81234567U;
                expected_flags = UINT_MAX - voice;
                expected_count = r == 0 ? 0 : (r == 1 ? 1 : 65535);
                expected_voices = (u8)(voice <= 32 ? voice : 32);
                expected_setting1 = (u8)(255 - voice);
                expected_setting2 = (u8)(voice ^ 0x5A);
                D_8004FA18 = D_8004FA19 = D_8004FA1A = 99;
                setup_result = results[r];
                if (kind == 0) {
                    actual = func_80010840(expected_frequency, (u8)voice,
                        expected_setting1, expected_setting2, expected_count, expected_flags);
                } else {
                    actual = func_800108E0(expected_frequency, (u8)voice,
                        expected_setting1, expected_setting2, expected_count, expected_flags);
                }
                assert(actual == setup_result);
                assert(events[0] == 10 + kind);
                if (setup_result == 0) { check_init(1); }
                else {
                    assert(event_count == 1);
                    assert(D_8002C630 == 0 && D_8004BE94 == reset_flag);
                }
                cases++;
            }
        }
    }
}
static void test_tick(void)
{
    static const u32 counters[] = {0, 1, 2, UINT_MAX};
    static const u32 reloads[] = {0, 1, UINT_MAX};
    static const u16 indices[] = {0, 1, 65535};
    int enabled, counter, reload, index, fired;
    u32 expected;
    D_80038228[0] = token; D_80038228[1] = token + 1;
    D_80038228[65535] = token + 2;
    for (enabled = 0; enabled < 3; enabled++) {
        for (counter = 0; counter < 4; counter++) {
            for (reload = 0; reload < 3; reload++) {
                for (index = 0; index < 3; index++) {
                    clear_events();
                    D_8002C630 = (u8)(enabled == 2 ? 255 : enabled);
                    D_800382A0 = counters[counter]; D_800382A4 = reloads[reload];
                    expected_count = indices[index];
                    expected_buffer = token + index;
                    expected = enabled ? counters[counter] - 1U : counters[counter];
                    fired = enabled && expected == 0;
                    if (fired) { expected = reloads[reload]; }
                    assert(func_80013D70(indices[index], expected_count) == 1);
                    assert(D_800382A0 == expected && D_800382A4 == reloads[reload]);
                    assert(D_8002C630 == (enabled == 2 ? 255 : enabled));
                    assert(event_count == (fired ? 3 : 1));
                    assert(events[event_count - 1] == 22);
                    cases++;
                }
            }
        }
    }
}
int main(void)
{
    assert(sizeof(u32) == 4 && sizeof(u16) == 2 && sizeof(u8) == 1);
    test_initialize(); test_wrappers(); test_tick();
    printf("PASS: %lu actual-source ABI, boundary and call-order cases\n", cases);
    return 0;
}
