/* Host-only contract doubles; matching source and archive compile separately. */
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

typedef struct OSMesgQueue_s { unsigned char storage[24]; } OSMesgQueue;
typedef struct AudioConfiguration {
    unsigned short duration;
    unsigned short delays[8];
    unsigned char gains[8];
    unsigned char count;
    unsigned char feedback;
    unsigned char mode;
    unsigned char unknown1D;
    short filter[4];
} AudioConfiguration;
typedef struct AudioDelayState {
    void *buffer;
    unsigned int length;
    unsigned short count;
    unsigned short feedback;
    unsigned int delays[8];
    unsigned short gains[8];
    unsigned int unknown3C;
    short filter[4];
} AudioDelayState;
extern void func_80010A40(void *);
extern void func_80010E80(AudioConfiguration *);
unsigned char D_80038290;
unsigned char D_80038291;
volatile unsigned char D_80038288;
unsigned short D_80038292;
unsigned int D_8003828C;
unsigned char *D_80038228[24];
OSMesgQueue D_800381F8;
void *(*D_80038018)(unsigned int, unsigned int);
void *D_800382F0;
AudioDelayState *D_800382E8;
unsigned int D_800382EC;
static unsigned char ring[24 * 768];
static AudioDelayState allocated;
static unsigned char command;
static unsigned int active_case;
static unsigned int trace[256];
static unsigned int trace_count;
static unsigned int zero_bytes;
static unsigned int expected_length;
static unsigned int receive_count;
static unsigned int played_count;
static unsigned int desired_events;
static unsigned int expected_buffers;
static unsigned int ignored_stop;
static unsigned int calls;
static unsigned int dynamic_case;
static unsigned int expected_index;
static void mark(unsigned int n) { assert(trace_count < 256); trace[trace_count++] = n; }
static void *allocate(unsigned int bytes, unsigned int alignment)
{
    calls++;
    if (active_case == 0) {
        assert(bytes == 768 * expected_buffers && alignment == 128);
        assert(D_80038292 == expected_buffers);
        mark(1);
        return ring;
    }
    assert(bytes == 72 && alignment == 0);
    mark(10);
    return &allocated;
}
void test_bzero(void *buffer, int bytes)
{
    unsigned int i;
    calls++;
    if (active_case == 0) {
        assert(buffer == ring && bytes == (int) (768 * expected_buffers));
        for (i = 0; i < expected_buffers; i++) assert(D_80038228[i] == ring + 768 * i);
        memset(buffer, 0, (unsigned int) bytes);
        mark(2);
    } else {
        assert(buffer == D_800382F0 && bytes == (int) (expected_length * 2U));
        zero_bytes = (unsigned int) bytes;
        mark(11);
    }
}
void osWritebackDCache(void *buffer, int bytes)
{
    calls++;
    if (active_case == 0) {
        assert(buffer == ring && bytes == (int) (768 * expected_buffers));
        assert(ring[0] == 0 && ring[bytes - 1] == 0);
        mark(3);
    } else if (buffer == D_800382F0) {
        assert(bytes == (int) (expected_length * 2U) && zero_bytes == (unsigned int) bytes);
        mark(12);
    } else {
        assert(buffer == &allocated && bytes == 72);
        assert(allocated.length == expected_length);
        mark(13);
    }
}
int osAiSetNextBuffer(void *buffer, unsigned int bytes)
{
    calls++;
    assert(active_case == 0 && bytes == 768);
    if (played_count == 0) expected_index = 0;
    else expected_index = (expected_index + 1) % D_80038292;
    assert(D_80038288 == expected_index);
    assert(buffer == ring + 768 * expected_index);
    played_count++;
    mark(4);
    return -1; /* The native thread deliberately ignores this return. */
}
int osRecvMesg(OSMesgQueue *queue, void **message, int flags)
{
    calls++;
    assert(active_case == 0 && queue == &D_800381F8 && flags == 1);
    assert(receive_count < desired_events + 4);
    if (receive_count == 0) {
        command = 255;
        D_80038291 = 1;
        ignored_stop++;
    } else {
        D_80038291 = 0;
        if (receive_count == 1) command = 77;
        else if (receive_count < desired_events + 2) command = 1;
        else command = 255;
    }
    if (dynamic_case) {
        if (receive_count == 2) D_80038292 = 1;
        if (receive_count == 7) D_80038292 = 24;
        if (receive_count == 30) D_80038292 = 5;
    }
    if (dynamic_case && receive_count == 0) *message = 0;
    else *message = &command;
    receive_count++;
    return 0;
}
void func_80014594(void)
{
    calls++;
    assert(active_case == 1 && D_800382EC == 91);
    assert(D_800382E8 != &allocated);
    mark(14);
}
void func_800145DC(void)
{
    calls++;
    assert(active_case == 1 && D_800382EC == 0);
    assert(D_800382E8 == &allocated);
    mark(15);
}
static void test_thread(void)
{
    unsigned int mode;
    unsigned int f;
    unsigned int i;
    unsigned int frequencies[] = {0, 22050, 22051, UINT_MAX};
    active_case = 0;
    for (mode = 0; mode < 2; mode++) {
        for (f = 0; f < 4; f++) {
            D_80038290 = (unsigned char) mode;
            D_8003828C = frequencies[f];
            expected_buffers = mode ? 24 : 16;
            if (frequencies[f] <= 22050) expected_buffers /= 2;
            desired_events = expected_buffers * 2 + 1;
            receive_count = played_count = ignored_stop = trace_count = 0;
            D_80038288 = 231;
            memset(ring, 0xA5, sizeof(ring));
            func_80010A40((void *) &command);
            assert(ignored_stop == 1 && receive_count == desired_events + 3);
            assert(played_count == desired_events + 1);
            assert(D_80038288 == desired_events % expected_buffers);
            assert(trace[0] == 1 && trace[1] == 2 && trace[2] == 3);
            for (i = 3; i < trace_count; i++) assert(trace[i] == 4);
        }
    }
}
static void test_dynamic_thread(void)
{
    active_case = 0;
    dynamic_case = 1;
    D_80038290 = 1;
    D_8003828C = 44100;
    expected_buffers = 24;
    desired_events = 52;
    receive_count = played_count = ignored_stop = trace_count = 0;
    D_80038288 = 231;
    memset(ring, 0xA5, sizeof(ring));
    func_80010A40((void *) &command);
    assert(ignored_stop == 1 && receive_count == desired_events + 3);
    assert(played_count == desired_events + 1);
    assert(D_80038292 == 5 && D_80038288 == expected_index);
    dynamic_case = 0;
}
static void test_configuration(void)
{
    AudioConfiguration configuration;
    AudioDelayState previous;
    unsigned int mode;
    unsigned int n;
    unsigned int f;
    unsigned int i;
    unsigned int frequencies[] = {0, 22050, 44100, UINT_MAX};
    unsigned int durations[] = {0, 1, 65535};
    unsigned int d;
    active_case = 1;
    D_800382F0 = 0;
    calls = 0;
    func_80010E80(0);
    assert(calls == 0); /* Guard precedes every dereference of the real input. */
    for (f = 0; f < 4; f++) {
        for (d = 0; d < 3; d++) {
            for (mode = 0; mode < 3; mode++) {
                for (n = 0; n <= 8; n++) {
                    memset(&configuration, 0, sizeof(configuration));
                    memset(&allocated, 0xA5, sizeof(allocated));
                    configuration.duration = (unsigned short) durations[d];
                    configuration.count = (unsigned char) n;
                    configuration.feedback = 255;
                    configuration.mode = (unsigned char) mode;
                    for (i = 0; i < 8; i++) {
                        configuration.delays[i] = (unsigned short) (i * 8191);
                        configuration.gains[i] = (unsigned char) (255 - i * 31);
                    }
                    configuration.filter[0] = -32768;
                    configuration.filter[1] = -1;
                    configuration.filter[2] = 12345;
                    configuration.filter[3] = 32767;
                    D_8003828C = frequencies[f];
                    D_800382F0 = ring;
                    D_800382E8 = &previous;
                    D_800382EC = 91;
                    expected_length = (configuration.duration * D_8003828C) / 1000U;
                    expected_length = 192U - expected_length % 192U + expected_length;
                    trace_count = zero_bytes = 0;
                    func_80010E80(&configuration);
                    assert(trace_count == 6);
                    for (i = 0; i < 6; i++) assert(trace[i] == 10 + i);
                    assert(allocated.buffer == ring && allocated.count == n);
                    assert(allocated.feedback == 65280);
                    for (i = 0; i < n; i++) {
                        unsigned int raw;
                        raw = configuration.delays[i] * D_8003828C / 1000U;
                        assert(allocated.delays[i] == ((raw + 3U) & 0xFFFCU));
                        assert(allocated.gains[i] == (unsigned short) (configuration.gains[i] << 8));
                    }
                    for (i = n; i < 8; i++) {
                        assert(allocated.delays[i] == 0xA5A5A5A5U);
                        assert(allocated.gains[i] == 0xA5A5);
                    }
                    assert(allocated.unknown3C == 0xA5A5A5A5U);
                    for (i = 0; i < 4; i++) {
                        assert(allocated.filter[i] == (mode == 1 ? configuration.filter[i] : (i == 3 ? 32767 : 0)));
                    }
                }
            }
        }
    }
}
int main(void)
{
    D_80038018 = allocate;
    test_thread();
    test_dynamic_thread();
    test_configuration();
    puts("PASS: 9 ring-thread scenarios including live counts and disabled null message; 324 configuration scenarios; disabled null-input guard");
    return 0;
}
