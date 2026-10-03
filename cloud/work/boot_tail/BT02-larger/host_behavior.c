/* Host-only contract doubles. Production matching sources compile separately. */
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct TransferRequest {
    void *destination;
    void *source;
    unsigned int size;
} TransferRequest;
typedef struct OSMesgQueue_s OSMesgQueue;
typedef struct OSThread_s OSThread;
typedef struct AudioCacheNode {
    struct AudioCacheNode *next;
    struct AudioCacheNode *previous;
} AudioCacheNode;
typedef struct AudioState {
    unsigned char active;
    unsigned char unknown01[7];
    double position;
    unsigned int current;
    unsigned int previous;
    unsigned char unknown18[8];
    unsigned short initial_count;
    unsigned char unknown22[6];
    unsigned short release_count;
    unsigned char unknown2A[0x1E];
    unsigned short count;
    unsigned short unknown4A;
    float value;
    unsigned int step;
    float scale;
    float saved_value;
    unsigned char state;
    unsigned char unknown5D[0xB];
} AudioState;

extern void func_80010628(void *, void *, unsigned int);
extern void func_80010714(void *, void *, unsigned int);
extern void func_80010C68(unsigned int *);
extern AudioCacheNode *func_80012660(unsigned int);
extern void func_80013C84(void);

volatile unsigned char D_80037FA0;
TransferRequest D_80037FA8[4];
unsigned char D_80037FE0[24];
unsigned char D_80038220;
unsigned char D_800381F8[24];
void *D_80038210[4];
unsigned int D_8003828C;
unsigned char *D_800381F0;
unsigned char D_80038040[512];
void *(*D_80038018)(unsigned int, unsigned int);
AudioCacheNode *D_80038354;
AudioCacheNode *D_80038358;
AudioCacheNode *D_8003835C;
AudioState *D_80038294;
unsigned short D_8003829C;

static int trace[16];
static int trace_count;
static void *expected_destination;
static unsigned int expected_size;
static unsigned int initial_count;
static unsigned int *expected_frequency;
static unsigned int frequency_result;
static unsigned char allocated_stack[1024];
static AudioState records[65535];

static void mark(int value)
{
    assert(trace_count < 16);
    trace[trace_count++] = value;
}
void func_80014594(void) { mark(1); }
void func_800105C4(void) { mark(2); }
void func_800145DC(void) { mark(4); }
void func_80010110(void) { mark(6); }
void osInvalDCache(void *destination, int bytes)
{
    mark(3);
    assert(destination == expected_destination);
    assert(bytes == (int) expected_size);
    assert(D_80037FA0 == initial_count + 1);
}
int osRecvMesg(OSMesgQueue *queue, void **message, int flags)
{
    mark(5);
    assert(queue == (OSMesgQueue *) D_80037FE0);
    assert(message == 0 && flags == 1);
    return -37;
}
void osCreateMesgQueue(OSMesgQueue *queue, void **messages, int count)
{
    mark(7);
    assert(D_80038220 == 1);
    assert(queue == (OSMesgQueue *) D_800381F8);
    assert(messages == D_80038210 && count == 4);
}
void osSetEventMesgAlt(int event, OSMesgQueue *queue, void *message)
{
    mark(8);
    assert(event == 6 && queue == (OSMesgQueue *) D_800381F8);
    assert(message == &D_80038220);
}
int osAiSetFrequency(unsigned int frequency)
{
    mark(9);
    assert(frequency == *expected_frequency);
    return (int) frequency_result;
}
static void *allocate_stack(unsigned int bytes, unsigned int alignment)
{
    mark(10);
    assert(bytes == 1024 && alignment == 128);
    assert(D_8003828C == frequency_result);
    assert(*expected_frequency == frequency_result);
    return allocated_stack;
}
void func_80010A40(void *argument) { (void) argument; }
void osCreateThread(OSThread *thread, int id, void (*entry)(void *),
                    void *argument, void *stack, int priority)
{
    mark(11);
    assert(thread == (OSThread *) D_80038040 && id == 0);
    assert(entry == func_80010A40 && argument == 0);
    assert(stack == allocated_stack + 1024 && priority == 122);
    assert(D_800381F0 == allocated_stack);
}
void osStartThread(OSThread *thread)
{
    mark(12);
    assert(thread == (OSThread *) D_80038040);
}
static void check_trace(const int *expected, int count)
{
    assert(trace_count == count);
    assert(memcmp(trace, expected, (size_t) count * sizeof(int)) == 0);
}
static void test_transfers(void)
{
    unsigned int i;
    unsigned int j;
    unsigned int k;
    unsigned int sizes[] = {0, 1, 15, 16, 17, UINT_MAX};
    unsigned int counts[] = {0, 1, 3, 4, 5, 255};
    int accepted_sync[] = {1, 2, 3, 4, 5, 6};
    int accepted_async[] = {1, 2, 3, 4};
    int rejected[] = {1, 2, 4};
    int destination;
    int source;
    TransferRequest before[4];
    for (i = 0; i < sizeof(counts) / sizeof(counts[0]); i++) {
        for (j = 0; j < sizeof(sizes) / sizeof(sizes[0]); j++) {
            for (k = 0; k < 2; k++) {
                memset(D_80037FA8, 0x5A, sizeof(D_80037FA8));
                memcpy(before, D_80037FA8, sizeof(before));
                D_80037FA0 = (unsigned char) counts[i];
                initial_count = counts[i];
                expected_destination = &destination;
                expected_size = sizes[j];
                trace_count = 0;
                if (k == 0) {
                    func_80010628(&destination, &source, sizes[j]);
                } else {
                    func_80010714(&destination, &source, sizes[j]);
                }
                if (counts[i] < 4) {
                    assert(D_80037FA8[counts[i]].destination == &destination);
                    assert(D_80037FA8[counts[i]].source == &source);
                    assert(D_80037FA8[counts[i]].size == ((sizes[j] + 15) & ~15U));
                    before[counts[i]] = D_80037FA8[counts[i]];
                    check_trace(k == 0 ? accepted_sync : accepted_async, k == 0 ? 6 : 4);
                } else {
                    assert(D_80037FA0 == counts[i]);
                    check_trace(rejected, 3);
                }
                assert(memcmp(before, D_80037FA8, sizeof(before)) == 0);
            }
        }
    }
}
static void test_initializer(void)
{
    unsigned int frequency;
    int expected[] = {7, 8, 9, 10, 11, 12};
    unsigned int values[] = {0, 22050, 48000, UINT_MAX};
    unsigned int i;
    D_80038018 = allocate_stack;
    for (i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        frequency = values[i];
        expected_frequency = &frequency;
        frequency_result = values[i] ^ 0x12345678U;
        D_80038220 = 255;
        trace_count = 0;
        func_80010C68(&frequency);
        assert(frequency == frequency_result);
        check_trace(expected, 6);
    }
}
static void test_lists(void)
{
    AudioCacheNode a;
    AudioCacheNode b;
    AudioCacheNode c;
    AudioCacheNode d;
    a.next = &b; a.previous = &d;
    b.next = 0; b.previous = &a;
    D_8003835C = &a; D_80038354 = 0; D_80038358 = 0;
    assert(func_80012660(0) == &a);
    assert(D_8003835C == &b && b.previous == 0);
    assert(D_80038354 == &a && D_80038358 == &a);
    assert(a.next == 0 && a.previous == &d);
    c.next = &d; c.previous = 0; d.next = 0; d.previous = &c;
    D_8003835C = &b; D_80038354 = &c; D_80038358 = &d;
    assert(func_80012660(UINT_MAX) == &b);
    assert(D_8003835C == 0 && D_80038358 == &b);
    assert(d.next == &b && b.previous == &d && b.next == 0);
    assert(func_80012660(123) == &c);
    assert(D_80038354 == &d && d.previous == 0);
    assert(D_80038358 == &c && b.next == &c);
    assert(c.previous == &b && c.next == 0);
    a.next = 0; a.previous = &d;
    b.next = 0; b.previous = &c;
    D_8003835C = &a; D_80038354 = &b; D_80038358 = 0;
    assert(func_80012660(7) == &a);
    assert(a.next == &b && b.previous == &a);
    assert(a.previous == &d && D_80038354 == &a && D_80038358 == &a);
}
static void test_counters(void)
{
    unsigned int i;
    AudioState before[6];
    double values[] = {0.0, 0.75, 1.5, 2147483647.5, 2147483648.5, 4294967295.75};
    assert(sizeof(AudioState) == 104);
    assert(offsetof(AudioState, position) == 8);
    assert(offsetof(AudioState, current) == 16);
    assert(offsetof(AudioState, previous) == 20);
    assert(offsetof(AudioState, initial_count) == 32);
    assert(offsetof(AudioState, state) == 92);
    D_80038294 = records;
    D_8003829C = 0;
    memset(records, 0, sizeof(records));
    func_80013C84();
    for (i = 0; i < 6; i++) {
        memset(&records[i], 0x5A, sizeof(records[i]));
        records[i].active = i == 1 ? 0 : 255;
        records[i].position = values[i];
        records[i].current = 100 + i;
        before[i] = records[i];
        if (records[i].active) {
            before[i].previous = records[i].current;
            before[i].current = (unsigned int) values[i];
        }
    }
    D_8003829C = 6;
    func_80013C84();
    assert(memcmp(before, records, sizeof(before)) == 0);
    memset(records, 0, sizeof(records));
    D_8003829C = 65535;
    records[65534].active = 1;
    records[65534].position = 321.75;
    records[65534].current = 987;
    func_80013C84();
    assert(records[65534].current == 321 && records[65534].previous == 987);
    assert(records[65533].current == 0 && records[65533].previous == 0);
}
int main(void)
{
    assert(sizeof(unsigned int) == 4);
    test_transfers(); test_initializer(); test_lists(); test_counters();
    puts("PASS: five actual sources; transfer, startup, list and numeric contracts");
    return 0;
}
