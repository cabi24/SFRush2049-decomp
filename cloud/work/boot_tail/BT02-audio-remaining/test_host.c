#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

typedef struct AudioConfiguration {
    unsigned int frequency;
    unsigned char mode;
    unsigned char sample_bits;
    unsigned char unknown06[256];
    signed char text[256];
} AudioConfiguration;
void func_80013DEC(void);
void func_80014198(unsigned int *, unsigned short, unsigned short, unsigned char);
void (*D_80038020)(void);
void (*D_80038024)(void);
unsigned char D_8002C630;
volatile unsigned char D_80038288;
unsigned short D_80038292;
unsigned char D_8003829E;
unsigned short D_800382CE;
unsigned short *D_800382D0;
unsigned short *D_800382D4;
unsigned short *D_800382D8[2];
unsigned short D_800382E0[2];
unsigned short D_80038360;
short D_80038362;
volatile unsigned char D_800382CD;
unsigned int D_800382A0;
unsigned int D_800382A4;
unsigned int D_8004F808;
AudioConfiguration D_8004F810;
signed char D_8002D890[256];
signed char D_8002D8A4[256];

static int events[70000];
static unsigned short seen_index[65536];
static unsigned short seen_offset[65536];
static unsigned int event_count;
static unsigned int seen_count;
static unsigned short buffers[2][16];
static int setup_call;
static unsigned int expected_frequency;
static unsigned short expected_count;
static unsigned short expected_size;
static unsigned char expected_mode;
static unsigned int *frequency_pointer;
static unsigned int frequency_step;

static void note(int value) { assert(event_count < 70000); events[event_count++] = value; }
static void before_callback(void) { note(0); }
static void middle_callback(void) { note(7); }
void func_80014624(void) { note(1); }
unsigned int __osDisableInt(void) { note(2); return 0xA5U; }
void __osRestoreInt(unsigned int value) { assert(value == 0xA5U); note(3); }
void func_8001DDE0(void) { note(4); }
void func_80011CD8(void) { note(5); }
void func_80011894(void) { note(6); }
void func_80012200(void) { note(8); }
void func_80013964(void)
{
    note(9);
    D_800382D4 = &D_800382E0[D_8003829E];
    D_800382D0 = D_800382D8[D_8003829E];
    *D_800382D4 = 0;
    *D_800382D0 = 0;
}
void func_80013C84(void) { note(10); }
int func_80013D70(unsigned short index, unsigned short offset)
{
    assert(seen_count < 65536);
    seen_index[seen_count] = index;
    seen_offset[seen_count++] = offset;
    note(11);
    return 1;
}
void func_80011D24(void) { note(12); }
void func_8001C508(void) { note(13); }
void func_800118C0(void) { note(14); }
void func_800119E0(void) { note(15); }
void func_8001C3CC(void) { note(16); }
void func_80014650(void) { note(17); }

void func_80011104(unsigned short count, unsigned int frequency)
{
    assert(setup_call++ == 0);
    assert(count == expected_count && frequency == expected_frequency);
    expected_frequency += frequency_step;
    *frequency_pointer = expected_frequency;
}
void func_800114C0(unsigned short count, unsigned int frequency, unsigned char mode)
{
    assert(setup_call++ == 1);
    assert(count == expected_count && frequency == expected_frequency && mode == expected_mode);
    expected_frequency += frequency_step;
    *frequency_pointer = expected_frequency;
}
void func_80010DD8(unsigned int frequency, unsigned short size)
{
    assert(setup_call++ == 2);
    assert(frequency == expected_frequency && size == expected_size);
    expected_frequency += frequency_step;
    *frequency_pointer = expected_frequency;
}
void func_800140F8(void)
{
    assert(setup_call++ == 3);
    expected_frequency += frequency_step;
    *frequency_pointer = expected_frequency;
}

static void expect_event(unsigned int *position, int event)
{
    assert(*position < event_count);
    assert(events[(*position)++] == event);
}

static void test_ring(unsigned short capacity, unsigned char published,
                      unsigned short previous, unsigned short limit,
                      unsigned int switches, short pending)
{
    unsigned int target, cursor, generated, expected_events, j;
    unsigned int target_previous;
    unsigned char old_buffer;
    int expected_pending;
    event_count = seen_count = 0;
    old_buffer = (unsigned char)(switches & 1);
    D_8002C630 = (unsigned char)((switches >> 1) & 1);
    D_80038024 = (switches & 4) ? before_callback : 0;
    D_80038020 = (switches & 8) ? middle_callback : 0;
    D_80038288 = published;
    D_80038292 = capacity;
    D_800382CE = limit;
    D_80038360 = previous;
    D_80038362 = pending;
    D_8003829E = old_buffer;
    D_800382D8[0] = buffers[0];
    D_800382D8[1] = buffers[1];
    D_800382E0[0] = D_800382E0[1] = 77;
    D_800382D0 = buffers[old_buffer];
    D_800382D4 = &D_800382E0[old_buffer];
    target = ((published ? published : capacity) - 1U) & 65535U;
    target_previous = previous;
    cursor = previous;
    generated = 0;
    if (previous == 65535U) {
        cursor = target;
    } else if (previous != target) {
        if (previous < target) {
            generated = target - previous;
            if (generated > limit) generated = limit;
            cursor = previous + generated;
        } else {
            j = previous < capacity ? capacity - previous : 0;
            if (j > limit) j = limit;
            generated = j;
            cursor = 0;
            j = target;
            if (j > (unsigned int)limit - generated) j = (unsigned int)limit - generated;
            generated += j;
            cursor = j;
        }
    }
    func_80013DEC();
    assert(seen_count == generated);
    for (j = 0; j < generated; j++) {
        unsigned int expected_index;
        expected_index = previous + j;
        if (previous >= target && expected_index >= capacity) expected_index -= capacity;
        assert(seen_index[j] == expected_index);
        assert(seen_offset[j] == (unsigned short)(j * 192U));
    }
    assert(D_80038360 == cursor);
    expected_pending = pending - (int)generated;
    assert(D_80038362 == (short)expected_pending);
    expected_events = 0;
    if (switches & 4) expect_event(&expected_events, 0);
    expect_event(&expected_events, 1);
    expect_event(&expected_events, 2);
    expect_event(&expected_events, 3);
    if (switches & 2) expect_event(&expected_events, 4);
    if (target_previous != 65535U) {
        expect_event(&expected_events, 5);
        expect_event(&expected_events, 6);
        if (switches & 8) expect_event(&expected_events, 7);
        expect_event(&expected_events, 8);
        expect_event(&expected_events, 9);
        if (target_previous != target) {
            expect_event(&expected_events, 10);
            for (j = 0; j < generated; j++) expect_event(&expected_events, 11);
            expect_event(&expected_events, 12);
        }
        expect_event(&expected_events, 13);
        expect_event(&expected_events, 14);
        expect_event(&expected_events, 15);
        if (switches & 2) expect_event(&expected_events, 16);
        assert(D_8003829E == (old_buffer ^ 1));
        assert(D_800382D4 == &D_800382E0[old_buffer ^ 1]);
        assert(D_800382D0 == buffers[old_buffer ^ 1]);
        assert(D_800382E0[old_buffer] == 0 && D_800382E0[old_buffer ^ 1] == 77);
    } else {
        assert(D_8003829E == old_buffer);
        assert(D_800382D4 == &D_800382E0[old_buffer]);
        assert(D_800382D0 == buffers[old_buffer]);
        assert(D_800382E0[0] == 77 && D_800382E0[1] == 77);
    }
    expect_event(&expected_events, 17);
    assert(expected_events == event_count);
}

static void test_setup(unsigned int frequency, unsigned short count, unsigned short size,
                       unsigned char mode, unsigned int length, unsigned int step)
{
    unsigned int i, blocks;
    float frame_samples;
    memset(&D_8004F810, 0x5A, sizeof(D_8004F810));
    memset(D_8002D890, 0x3F, sizeof(D_8002D890));
    D_8002D890[length] = 0;
    for (i = 0; i < 256; i++) D_8002D8A4[i] = (signed char)(i ^ 0xA5);
    D_800382CD = 255;
    D_800382A0 = D_800382A4 = D_8004F808 = 0xDEADBEEFU;
    expected_count = count > 32 ? 32 : count;
    expected_size = size;
    expected_mode = mode;
    expected_frequency = frequency;
    frequency_pointer = &frequency;
    frequency_step = step;
    setup_call = 0;
    func_80014198(&frequency, count, size, mode);
    assert(setup_call == 4 && D_800382CD == 0);
    frame_samples = (float)frequency / 60.0f;
    blocks = (unsigned int)(frame_samples / 192.0f + 0.5f);
    assert(D_800382A0 == blocks && D_800382A4 == blocks && D_8004F808 == blocks * 192U);
    assert(D_8004F810.frequency == frequency && D_8004F810.mode == 1 && D_8004F810.sample_bits == 16);
    for (i = 0; i < 256; i++) {
        assert(D_8004F810.unknown06[i] == 0x5A);
        if (i < length) assert(D_8004F810.text[i] == D_8002D8A4[i]);
        else if (i == length) assert(D_8004F810.text[i] == 0);
        else assert(D_8004F810.text[i] == 0x5A);
    }
}

int main(void)
{
    unsigned int capacity, published, previous, limit, switches, cases, i;
    unsigned int frequencies[] = { 0U, 1U, 5759U, 5760U, 5761U, 22050U, 32000U, 44100U, 48000U, 2147483647U, 2147483648U, UINT_MAX };
    unsigned short counts[] = { 0, 1, 31, 32, 33, 65535 };
    cases = 0;
    for (capacity = 1; capacity <= 8; capacity++) {
        for (published = 0; published <= capacity; published++) {
            for (previous = 0; previous <= capacity; previous++) {
                for (limit = 0; limit <= capacity + 1; limit++) {
                    for (switches = 0; switches < 16; switches++) {
                        test_ring((unsigned short)capacity, (unsigned char)published,
                                  previous == capacity ? 65535 : (unsigned short)previous,
                                  (unsigned short)limit, switches, -30000);
                        cases++;
                    }
                }
            }
        }
    }
    test_ring(65535, 0, 0, 65535, 15, 32767); cases++;
    test_ring(4096, 0, 0, 4096, 15, 0); cases++;
    test_ring(65535, 1, 65000, 1024, 15, -32768); cases++;
    printf("ring cases: %u\n", cases);
    cases = 0;
    for (i = 0; i < sizeof(frequencies) / sizeof(frequencies[0]); i++) {
        for (capacity = 0; capacity < sizeof(counts) / sizeof(counts[0]); capacity++) {
            for (published = 0; published < 256; published++) {
                test_setup(frequencies[i], counts[capacity], (unsigned short)(published * 257U),
                           (unsigned char)published, published, 0);
                cases++;
            }
        }
    }
    test_setup(48000, 33, 65535, 255, 255, 1); cases++;
    printf("setup cases: %u\n", cases);
    return 0;
}
