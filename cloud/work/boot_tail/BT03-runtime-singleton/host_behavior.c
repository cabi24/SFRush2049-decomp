#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nonmatch/func_80018634.c"
Context *D_8004BE80;
static Context a, b, expected_a, expected_b;
static SequenceEvent events[64][4];
static unsigned char *resource;
static int calls, mode;
static int kinds[256];
static Context *contexts[256];
static u32 arguments[256][2];
static void record(int kind, Context *context, u32 x, u32 y)
{
    assert(calls < 256);
    kinds[calls] = kind;
    contexts[calls] = context;
    arguments[calls][0] = x;
    arguments[calls][1] = y;
    calls++;
}
void func_80017720(Context *context, u8 program, u8 channel)
{
    record(1, context, program, channel);
    if (mode == 1) {
        assert(a.playback[0].cursor == resource + 0x10C);
        D_8004BE80 = &b;
    }
}
void func_80017824(u8 value, u8 channel) { record(2, 0, value, channel); }
void func_80018B3C(u32 value)
{
    record(3, 0, value, 0);
    if (mode == 2) D_8004BE80 = &b;
}
static void setup(void)
{
    int i;
    SequenceHeader *header;
    memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b));
    memset(events, 0, sizeof(events)); memset(resource, 0, 4096);
    header = (SequenceHeader *)resource;
    header->patterns = 0x40; header->channels = 0x80; header->loop_time = 10;
    ((u32 *)(resource + 0x40))[0] = 0x100;
    ((u32 *)(resource + 0x40))[1] = 0x140;
    for (i = 0; i < 64; i++) resource[0x80 + i] = i % 16;
    a.sequence = b.sequence = header;
    D_8004BE80 = &a; calls = 0; mode = 0;
}
static void activate(int i, u32 time, u16 code)
{
    events[i][0].time = time; events[i][0].code = code;
    events[i][0].program = events[i][0].controller = 255;
    a.tracks[i].start = a.tracks[i].cursor = events[i];
}
static void assert_contexts(void)
{
    assert(memcmp(&a, &expected_a, sizeof(a)) == 0);
    assert(memcmp(&b, &expected_b, sizeof(b)) == 0);
}
int main(void)
{
    int i, bits, trial;
    unsigned int total;
    Pattern *pattern;
    Playback *play;
    resource = (unsigned char *)malloc(4096);
    assert(resource != 0);
    setup(); expected_a = a; expected_b = b;
    assert(func_80018634() == 0 && calls == 0); assert_contexts();
    for (trial = 0; trial < 4; trial++) {
        setup(); a.step_fraction = trial == 3 ? 0xFFFFFFFFU : (u32)trial * 65535U;
        a.step_time = trial == 2 ? 0xFFFFFFFFU : (u32)trial;
        a.lookahead = 123;
        for (i = 0; i < 64; i++) {
            activate(i, 100000, 0xFFFF);
            a.tracks[i].fraction = 0xFFFFFF00U + (u32)i * 7U;
            a.tracks[i].time = i;
        }
        expected_a = a; expected_b = b;
        for (i = 0; i < 64; i++) {
            total = a.tracks[i].fraction + a.step_fraction;
            expected_a.tracks[i].fraction = total & 65535U;
            expected_a.tracks[i].time += (total >> 16) + a.step_time;
        }
        assert(func_80018634() == 1 && calls == 0); assert_contexts();
    }
    for (trial = 0; trial < 2; trial++) {
        setup(); a.stop_loop = trial;
        for (i = 0; i < 64; i++) activate(i, 0, trial ? 0xFFFE : 0xFFFF);
        expected_a = a; expected_b = b;
        for (i = 0; i < 64; i++) expected_a.tracks[i].cursor = 0;
        assert(func_80018634() == 1 && calls == 0); assert_contexts();
        assert(func_80018634() == 0 && calls == 0); assert_contexts();
    }
    for (bits = 0; bits < 16; bits++) {
        setup(); memset(a.playback, 0xA5, sizeof(a.playback));
        activate(63, 0, 0); events[63][1].time = 1000;
        events[63][0].program = bits & 4 ? 7 : 255;
        events[63][0].controller = bits & 8 ? 9 : 255;
        events[63][0].value.bytes.first = -128;
        events[63][0].value.bytes.second = 127;
        pattern = (Pattern *)(resource + 0x100);
        pattern->optional04 = bits & 1 ? 0x300 : 0;
        pattern->optional08 = bits & 2 ? 0x340 : 0;
        expected_a = a; expected_b = b; play = &expected_a.playback[63];
        play->counter00 = play->counter04 = play->counter08 = 0;
        play->counter1C = play->counter20 = 0;
        play->cursor = resource + 0x10C;
        play->optional10 = bits & 1 ? resource + 0x300 : 0;
        play->optional14 = bits & 2 ? resource + 0x340 : 0;
        play->value18 = 0x2000; play->value1A = 0;
        play->channel = 15; play->first = -128; play->second = 127; play->index = 63;
        expected_a.tracks[63].cursor++;
        assert(func_80018634() == 1); assert_contexts();
        assert(calls == ((bits & 4) != 0) + ((bits & 8) != 0));
        i = 0;
        if (bits & 4) { assert(kinds[i] == 1 && contexts[i] == &a && arguments[i][0] == 7 && arguments[i][1] == 15); i++; }
        if (bits & 8) assert(kinds[i] == 2 && arguments[i][0] == 9 && arguments[i][1] == 15);
    }
    setup(); a.loop_count = 65535; a.step_fraction = 65535; a.step_time = 2;
    for (i = 0; i < 64; i++) {
        activate(i, 0, 0xFFFE); events[i][0].value.jump = 1; events[i][1].time = 1000;
    }
    expected_a = a; expected_b = b; expected_a.loop_count = 0;
    for (i = 0; i < 64; i++) {
        expected_a.tracks[i].cursor++;
        expected_a.tracks[i].fraction = 65535;
        expected_a.tracks[i].time = 12;
    }
    assert(func_80018634() == 1); assert_contexts();
    assert(calls == 1 && kinds[0] == 3 && arguments[0][0] == 10);
    setup(); mode = 1; activate(0, 0, 0);
    events[0][0].program = 7; events[0][0].controller = 9; events[0][1].time = 1000;
    b.tracks[0].cursor = events[0]; b.playback[0].channel = 11;
    expected_a = a; expected_b = b;
    expected_a.playback[0].cursor = resource + 0x10C; expected_a.playback[0].value18 = 0x2000;
    expected_b.tracks[0].cursor++;
    assert(func_80018634() == 1 && D_8004BE80 == &b); assert_contexts();
    assert(calls == 2 && kinds[0] == 1 && arguments[0][1] == 0 && kinds[1] == 2 && arguments[1][0] == 9 && arguments[1][1] == 11);
    setup(); mode = 2; activate(0, 0, 0xFFFE); events[0][0].value.jump = 1; events[0][1].time = 1000;
    b.tracks[0].cursor = events[0] + 1; b.loop_count = 40;
    expected_a = a; expected_b = b;
    expected_a.tracks[0].cursor++; expected_a.tracks[0].time = 10; expected_b.loop_count = 41;
    assert(func_80018634() == 1 && D_8004BE80 == &b); assert_contexts();
    assert(calls == 1 && kinds[0] == 3 && arguments[0][0] == 10);
    free(resource); puts("PASS: 26 synthetic scenarios plus two inactive follow-through checks, full-context preservation and callback replacement");
    return 0;
}
