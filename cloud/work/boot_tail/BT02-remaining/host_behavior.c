/* Host-only callback doubles. Matching sources are separate translation units. */
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef struct OSMesgQueue_s OSMesgQueue;
unsigned char D_8002C630;
volatile unsigned char D_800382CD;
volatile unsigned char D_800382CC;
unsigned char D_800382B0[24];
void *D_800382E8;
void *D_800382F0;
void (*D_8003801C)(void *);
void (*D_80038004)(void);

extern void func_80010980(void);
extern void func_800109C0(void);
extern void func_80011074(void);
extern void func_800110C4(void);
extern void func_800118C0(void);
extern void func_80011CD8(void);
extern void func_80014488(void);
extern void func_800144F0(void);

static unsigned int events[32];
static unsigned int count;
static int object_a, object_b, object_c;
static int replace_buffer;
static int queue_path;
static int clear_active_in_helper;

static void emit(unsigned int event)
{
    assert(count < sizeof(events) / sizeof(events[0]));
    events[count++] = event;
}

static void stop_callback(void)
{
    assert(D_800382CD != 0);
    emit(2);
    if (replace_buffer) {
        D_800382E8 = &object_b;
    }
}

static void release_callback(void *object)
{
    if (object == &object_c) {
        emit(5);
        assert(D_800382F0 == &object_c);
    } else {
        emit(4);
        assert(object == (replace_buffer ? &object_b : &object_a));
        assert(D_800382E8 == object);
        assert(D_800382CD == 0);
    }
}

void func_80010110(void)
{
    emit(3);
    if (queue_path) {
        assert(D_800382CC == 0);
    } else {
        assert(D_800382CD != 0);
    }
}

int osRecvMesg(OSMesgQueue *queue, void **message, int flags)
{
    emit(12);
    assert((void *)queue == (void *)D_800382B0);
    assert(message == 0);
    assert(flags == 1);
    assert(D_800382CC != 0);
    return -7; /* The actual wrapper ignores the result. */
}

void func_80014140(void)
{
    emit(1);
    if (clear_active_in_helper) {
        D_8002C630 = 0;
    }
}
void func_80011848(void) { emit(6); }
void func_800121BC(void) { emit(7); }
void func_8001261C(void) { emit(8); }
void func_8001144C(void) { emit(9); }
void func_80010D74(void) { emit(10); }
void func_8001E0D4(void) { assert(D_8002C630 != 0); emit(11); }

static void reset(void)
{
    count = 0;
    memset(events, 0, sizeof(events));
    D_8002C630 = 0;
    D_800382CD = 0;
    D_800382CC = 0;
    D_800382E8 = 0;
    D_800382F0 = 0;
    D_8003801C = release_callback;
    D_80038004 = stop_callback;
    replace_buffer = 0;
    queue_path = 0;
    clear_active_in_helper = 0;
}

static void expect(const unsigned int *expected, unsigned int length)
{
    assert(count == length);
    assert(memcmp(events, expected, length * sizeof(events[0])) == 0);
}

int main(void)
{
    static const unsigned int stop[] = {2, 3};
    static const unsigned int release_a[] = {4};
    static const unsigned int stopped_release[] = {2, 3, 4};
    static const unsigned int release_pair[] = {4, 5};
    static const unsigned int release_second[] = {5};
    static const unsigned int queue[] = {12, 3};
    static const unsigned int full[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    static const unsigned int short_full[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 11};
    static const unsigned int group[] = {1, 6, 7, 8, 9, 10};
    static const unsigned int short_group[] = {1, 6, 7, 8, 9};

    reset();
    func_80010980(); func_800109C0(); func_80014488(); func_800144F0();
    func_80011074(); func_800110C4(); func_800118C0(); func_80011CD8();
    assert(count == 0);

    reset(); D_800382CD = 255;
    func_800118C0(); expect(stop, 2); assert(D_800382CD == 0);
    func_800118C0(); expect(stop, 2);

    reset(); D_800382E8 = &object_a;
    func_80011074(); expect(release_a, 1); assert(D_800382E8 == 0);
    func_80011074(); expect(release_a, 1);

    reset(); D_800382E8 = &object_a; D_800382CD = 1; replace_buffer = 1;
    func_80011074(); expect(stopped_release, 3); assert(D_800382E8 == 0);

    reset(); D_800382E8 = &object_a; D_800382F0 = &object_c;
    func_800110C4(); expect(release_pair, 2);
    assert(D_800382E8 == 0 && D_800382F0 == &object_c);
    count = 0; func_800110C4(); expect(release_second, 1);
    assert(D_800382F0 == &object_c);

    reset(); D_800382CC = 255; queue_path = 1;
    func_80011CD8(); expect(queue, 2); assert(D_800382CC == 0);
    func_80011CD8(); expect(queue, 2);

    reset(); D_8002C630 = 255; D_800382CD = 1;
    D_800382E8 = &object_a; D_800382F0 = &object_c;
    func_80010980(); expect(full, 11);
    assert(D_8002C630 == 0 && D_800382CD == 0 && D_800382E8 == 0);
    assert(D_800382F0 == &object_c);
    func_80010980(); expect(full, 11);

    reset(); D_8002C630 = 1; D_800382CD = 1;
    D_800382E8 = &object_a; D_800382F0 = &object_c;
    func_800109C0(); expect(short_full, 10); assert(D_8002C630 == 0);
    func_800109C0(); expect(short_full, 10);

    reset(); D_8002C630 = 255;
    func_80014488(); expect(group, 6); assert(D_8002C630 == 255);
    reset(); D_8002C630 = 255;
    func_800144F0(); expect(short_group, 5); assert(D_8002C630 == 255);

    reset(); D_8002C630 = 1; clear_active_in_helper = 1;
    func_80014488(); expect(group, 6); assert(D_8002C630 == 0);
    reset(); D_8002C630 = 1; clear_active_in_helper = 1;
    func_800144F0(); expect(short_group, 5); assert(D_8002C630 == 0);

    puts("PASS: all eight real sources, zero/nonzero gates, nested call order, callback reload, queue ABI and state timing");
    return 0;
}
