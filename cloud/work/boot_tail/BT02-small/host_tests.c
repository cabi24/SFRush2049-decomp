/* Host-only behavior checks; this file is never an IDO matching candidate. */
#include <assert.h>
#include <stddef.h>
#include <string.h>

unsigned char D_8002C630;
unsigned char D_8004F810[4];
void (*D_80038020)(void);
unsigned short D_80038028;
unsigned int D_8003802C, D_80038038, D_8003803C, D_8003828C;
void *D_800382D0;
unsigned short *D_800382D4;

typedef struct AudioState {
    unsigned char unknown00[0x20];
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
} AudioState;
typedef struct AudioNode {
    struct AudioNode *next;
    unsigned char unknown04[12];
    unsigned short countdown;
} AudioNode;
AudioNode *D_80038344;

void func_8001061C(void);
void func_80010A0C(unsigned int);
void *func_80010A14(void);
void func_80010D3C(unsigned int *);
void func_80011894(void);
void func_800119E0(void);
void func_80011A10(AudioState *);
void func_80011A3C(AudioState *);
void func_80012200(void);

static unsigned int frequency_argument, frequency_calls;
static void *task_argument;
static unsigned short task_count;
static unsigned int task_calls;

unsigned int func_8000BF00(unsigned int frequency)
{
    frequency_argument = frequency;
    ++frequency_calls;
    return frequency ^ 0x13579BDFU;
}
void func_80011910(void *data, unsigned short count)
{
    task_argument = data;
    task_count = count;
    ++task_calls;
}
static void callback(void) {}

int main(void)
{
    const unsigned int values[] = {0U, 1U, 15U, 16U, 0xFFFFFFFFU};
    unsigned int i, frequency;
    unsigned short count;
    unsigned char task_data[4];
    AudioState audio, expected;
    AudioNode a, b, c;

    assert(sizeof(unsigned int) == 4 && sizeof(unsigned short) == 2);
    assert(offsetof(AudioState, count) == 0x48);
    assert(offsetof(AudioState, state) == 0x5C);
    D_80038020 = callback;
    func_8001061C();
    assert(D_80038020 == 0);
    func_8001061C();
    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        func_80010A0C(values[i]);
        D_8002C630 = (unsigned char)values[i];
        assert(func_80010A14() == (D_8002C630 ? D_8004F810 : 0));
        frequency = values[i];
        func_80010D3C(&frequency);
        assert(frequency_argument == values[i]);
        assert(frequency == (values[i] ^ 0x13579BDFU));
        assert(D_8003828C == frequency);
        D_8003802C = values[i];
        D_80038028 = 0xFFFF;
        func_80011894();
        assert(D_8003802C == values[i]);
        assert(D_80038038 == values[i]);
        assert(D_8003803C == (values[i] & ~15U));
        assert(D_80038028 == 0);
        count = (unsigned short)values[i];
        D_800382D0 = task_data;
        D_800382D4 = &count;
        func_800119E0();
        assert(task_argument == task_data && task_count == count);
    }
    assert(frequency_calls == 5 && task_calls == 5);
    memset(&audio, 0xA5, sizeof(audio));
    audio.initial_count = 0xFFFF;
    expected = audio;
    expected.state = 0;
    expected.step = 0;
    expected.saved_value = 0.0f;
    expected.value = 0.0f;
    expected.count = expected.initial_count;
    expected.scale = 1.0f;
    func_80011A10(&audio);
    assert(memcmp(&audio, &expected, sizeof(audio)) == 0);
    audio.release_count = 123;
    audio.value = -3.5f;
    expected = audio;
    expected.state = 3;
    expected.step = 0;
    expected.saved_value = expected.value;
    expected.scale = 0.0f;
    expected.count = expected.release_count;
    func_80011A3C(&audio);
    assert(memcmp(&audio, &expected, sizeof(audio)) == 0);
    D_80038344 = 0;
    func_80012200();
    memset(&a, 0xA5, sizeof(a));
    memset(&b, 0xA5, sizeof(b));
    memset(&c, 0xA5, sizeof(c));
    a.next = &b; b.next = &c; c.next = 0;
    a.countdown = 0; b.countdown = 1; c.countdown = 0xFFFF;
    D_80038344 = &a;
    func_80012200();
    assert(a.countdown == 0 && b.countdown == 0 && c.countdown == 0xFFFE);
    func_80012200();
    assert(a.countdown == 0 && b.countdown == 0 && c.countdown == 0xFFFD);
    assert(a.next == &b && b.next == &c && c.next == 0 && D_80038344 == &a);
    for (i = 0; i < sizeof(a.unknown04); ++i)
        assert(a.unknown04[i] == 0xA5 && b.unknown04[i] == 0xA5 && c.unknown04[i] == 0xA5);
    return 0;
}
