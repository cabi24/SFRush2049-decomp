/* Test-only doubles; actual matching C is compiled as separate translation units. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct OSMesgQueue_s OSMesgQueue;
typedef void *OSMesg;
typedef struct AudioState {
    unsigned char active;
    unsigned char unknown01[0x1F];
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

extern void func_800105C4(void);
extern void func_80010D74(void);
extern void func_80011C84(unsigned short);
extern unsigned char func_80014374(unsigned int);
extern int func_800143C0(unsigned int *, unsigned short, unsigned short, unsigned int);
extern int func_80014434(unsigned int *, unsigned short, unsigned short, unsigned int);

unsigned char D_80037FE0[24], D_800381F8[24];
OSMesg D_80037FF8[1];
volatile unsigned char D_80037FA0;
void (*D_80038020)(void);
void (*D_8003801C)(void *);
void *D_80038228, *D_800381F0;
AudioState *D_80038294;
unsigned char D_80038290, D_80038291;
static AudioState records[65536];
static int calls[8], ncall, create_count, setup_kind;
static int tokens[3];
static unsigned int *expected_frequency;
static unsigned short expected_count, expected_size;
static unsigned char expected_mode, expected_flag;
static AudioState *expected_record;

void func_80010450(void) { assert(0); }
static void existing_callback(void) { assert(0); }
void osCreateMesgQueue(OSMesgQueue *q, OSMesg *buffer, int n)
{
    assert((void *)q == (void *)D_80037FE0);
    assert(buffer == D_80037FF8 && n == 1);
    assert(D_80038020 == 0 && D_80037FA0 == 255);
    ++create_count;
}
static void release_second(void *p)
{
    assert(p == &tokens[2]);
    calls[ncall++] = 3;
}
static void release_first(void *p)
{
    assert(p == &tokens[0]);
    calls[ncall++] = 2;
    D_8003801C = release_second;
    D_800381F0 = &tokens[2];
}
int osJamMesg(OSMesgQueue *q, OSMesg message, int block)
{
    assert((void *)q == (void *)D_800381F8);
    assert(*(unsigned char *)message == 255 && block == 1);
    calls[ncall++] = 1;
    return -1;
}
void func_80011A10(AudioState *audio)
{
    assert(audio == expected_record && audio->active == 7);
    audio->state = 0;
    /* The caller must retain its selected record even if the global changes. */
    D_80038294 = 0;
    calls[ncall++] = 4;
}
void func_80014550(void)
{
    assert(D_80038291 == (setup_kind == 1 ? 0 : 9));
    calls[ncall++] = 5;
}
void func_80010C68(unsigned int *frequency)
{
    assert(setup_kind == 1 && frequency == expected_frequency);
    assert(D_80038290 == expected_flag && D_80038291 == 0);
    *frequency += 1;
    calls[ncall++] = 6;
}
void func_80010D3C(unsigned int *frequency)
{
    assert(setup_kind == 2 && frequency == expected_frequency);
    assert(D_80038290 == 8 && D_80038291 == 9);
    *frequency += 2;
    calls[ncall++] = 7;
}
void func_80014198(unsigned int *frequency, unsigned short count,
                   unsigned short size, unsigned char mode)
{
    assert(frequency == expected_frequency && *frequency == 48000U + (unsigned int)setup_kind);
    assert(count == expected_count && size == expected_size && mode == expected_mode);
    calls[ncall++] = 8;
}
static unsigned char model(unsigned int flags)
{
    if ((flags & 0x10000U) != 0) return 2;
    if ((flags & 0x20000U) != 0) return 3;
    if ((flags & 0x40000U) != 0) return 4;
    if ((flags & 0x80000U) != 0) return 0;
    return 1;
}
int main(void)
{
    unsigned int i, flags, frequency, j;
    unsigned short indexes[3];
    assert(sizeof(AudioState) == 104);
    assert(offsetof(AudioState, initial_count) == 0x20);
    assert(offsetof(AudioState, count) == 0x48);
    assert(offsetof(AudioState, state) == 0x5C);
    D_80038020 = 0;
    D_80037FA0 = 255;
    func_800105C4();
    assert(create_count == 1 && D_80037FA0 == 0 && D_80038020 == func_80010450);
    D_80037FA0 = 42;
    func_800105C4();
    assert(create_count == 1 && D_80037FA0 == 42);
    D_80038020 = existing_callback;
    func_800105C4();
    assert(create_count == 1 && D_80038020 == existing_callback && D_80037FA0 == 42);
    D_8003801C = release_first;
    D_80038228 = &tokens[0];
    D_800381F0 = &tokens[1];
    ncall = 0;
    func_80010D74();
    assert(ncall == 3 && calls[0] == 1 && calls[1] == 2 && calls[2] == 3);
    assert(D_80038228 == &tokens[0] && D_800381F0 == &tokens[2]);
    indexes[0] = 0; indexes[1] = 17; indexes[2] = 65535;
    for (i = 0; i < 3; ++i) {
        D_80038294 = records;
        expected_record = &records[indexes[i]];
        memset(expected_record, 7, sizeof(*expected_record));
        ncall = 0;
        func_80011C84(indexes[i]);
        assert(expected_record->active == 1 && expected_record->state == 0);
        assert(expected_record->unknown01[0] == 7 && D_80038294 == 0);
        assert(ncall == 1 && calls[0] == 4);
    }
    for (i = 0; i < 32; ++i) {
        for (j = 0; j < 2; ++j) {
            flags = (i << 16) | (j ? 0xFFE0FFFFU : 0);
            expected_mode = model(flags);
            assert(func_80014374(flags) == expected_mode);
            expected_flag = (unsigned char)((flags & 0x100000U) != 0);
            expected_count = (unsigned short)(i == 0 ? 0 : 65535);
            expected_size = (unsigned short)(i == 31 ? 0 : 65535);
            expected_frequency = &frequency;
            for (setup_kind = 1; setup_kind <= 2; ++setup_kind) {
                frequency = 48000;
                D_80038290 = 8; D_80038291 = 9;
                ncall = 0;
                if (setup_kind == 1) {
                    assert(func_800143C0(&frequency, expected_count, expected_size, flags) == 0);
                } else {
                    assert(func_80014434(&frequency, expected_count, expected_size, flags) == 0);
                }
                assert(ncall == 3 && calls[0] == 5 && calls[1] == setup_kind + 5 && calls[2] == 8);
            }
        }
    }
    puts("PASS: one-time queue setup, jam/reload order, 104-byte record bounds, 64 flag combinations and 128 setup calls");
    return 0;
}
