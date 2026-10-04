#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AudioState {
    unsigned char active;
    unsigned char unknown01[0x5F];
    unsigned char bit_index;
    unsigned char changed;
    unsigned char unknown62[6];
} AudioState;
typedef struct AudioLoop { unsigned int unknown00; int length; } AudioLoop;
typedef struct AudioCommand {
    unsigned char unknown00[44];
    unsigned short status;
    unsigned char unknown2E[22];
    void *output;
    unsigned char unknown48[8];
} AudioCommand;
typedef struct AudioBlock {
    unsigned short count;
    unsigned short sample;
    unsigned int changed;
    void *work;
    AudioLoop *loop;
    AudioCommand commands[32];
} AudioBlock;
typedef struct OSMesgQueue { unsigned char opaque[24]; } OSMesgQueue;
typedef void *OSMesg;
AudioState *D_80038294, *D_80038298;
unsigned short D_8003829C, D_800382CE, D_800382A8, D_800382E0[2];
unsigned char D_8003829E, D_80038290;
float D_8002D8B8, D_8002D8BC, D_8002D8C0, D_8002D8C4, D_8002D8C8;
void *D_800382D8, *D_800382DC, *D_800382E4, *D_8003802C;
unsigned int D_800382AC;
OSMesgQueue D_800382B0;
OSMesg D_800382C8;
volatile unsigned char D_800382CC;
unsigned short D_8002C5D0[256];
AudioBlock *D_800382D0;
unsigned short *D_800382D4;
AudioLoop *D_800382E8;
int D_800382EC;
void *D_80038030, *D_80038034;
extern void func_80011104(unsigned short, unsigned int);
extern void func_800114C0(unsigned short, unsigned int, unsigned char);
extern void func_800139D4(void *, unsigned short);
static AudioState states[65535];
static unsigned char opaque[4][1024];
static AudioBlock blocks[3];
static unsigned int allocations[4];
static int phase, alloc_count, call_count, calls[8], seen_count, expected_samples;
static unsigned char seen_active[65535];
static unsigned short seen_index[65535];
static int cache_count, callback_count;
static void *allocate(unsigned int size, unsigned int alignment)
{
    assert(alignment == 0);
    allocations[alloc_count] = size;
    alloc_count++;
    if (phase == 1 && alloc_count == 1) {
        assert(D_8003829C * sizeof(AudioState) == size);
        return states;
    }
    assert(alloc_count <= 4);
    return opaque[alloc_count - 1];
}
void *(*D_80038018)(unsigned int, unsigned int) = allocate;
void func_80008590(void *p, int n)
{
    assert(phase == 1 && p == D_800382E4 && n == 664);
    assert(alloc_count == 4 && cache_count == 0);
    memset(p, 0, (size_t)n);
    cache_count++;
}
void func_80007CA0(void *p, int n)
{
    assert(phase == 1 && p == D_800382E4 && n == 664);
    assert(cache_count == 1 && ((unsigned char *)p)[0] == 0);
    cache_count++;
}
void func_80006A00(OSMesgQueue *q, OSMesg *m, int count)
{
    assert(phase == 2 && q == &D_800382B0 && m == &D_800382C8 && count == 1);
    assert(alloc_count == 1 && D_8003802C == opaque[0]);
    assert(D_800382CC == 0x7f);
    calls[call_count++] = 1;
}
void func_80011F60(int count)
{
    assert(phase == 2 && D_800382CC == 0 && count == D_800382A8);
    calls[call_count++] = 2;
}
void func_800123A8(unsigned short count)
{
    assert(phase == 2 && D_800382CC == 0);
    callback_count = count;
    calls[call_count++] = 3;
}
void func_80012D18(AudioState *state, unsigned short samples, unsigned char active)
{
    assert(phase == 3 && state >= states && state < states + D_8003829C);
    assert(samples == expected_samples);
    assert(state->active != 0 && state->changed == 0);
    seen_active[seen_count] = active;
    seen_index[seen_count] = (unsigned short)(state - states);
    seen_count++;
}
static unsigned int test_init(void)
{
    unsigned short counts[] = {0, 1, 32, 65535};
    unsigned int frequencies[] = {0, 11025, 44100, 96000, 2147483648U};
    float factors[] = {0.5f, 1.0f, 1.75f};
    unsigned int a, b, c, mode, i, cases;
    unsigned short expected;
    cases = 0;
    phase = 1;
    for (a = 0; a < 4; a++) for (b = 0; b < 5; b++)
    for (c = 0; c < 3; c++) for (mode = 0; mode < 2; mode++) {
        memset(states, 0xa5, sizeof(states));
        memset(opaque, 0xa5, sizeof(opaque));
        D_80038290 = mode ? 255 : 0;
        D_8002D8B8 = factors[c]; D_8002D8BC = factors[2 - c];
        D_8003829E = 17; D_800382E0[0] = 15; D_800382E0[1] = 18;
        expected = (unsigned short)((unsigned int)(((float)frequencies[b] /
            (mode ? 25.0f : 50.0f) / 192.0f) * (mode ? D_8002D8BC : D_8002D8B8)) + 1);
        alloc_count = cache_count = 0;
        func_80011104(counts[a], frequencies[b]);
        assert(D_8003829C == counts[a] && D_80038294 == states && D_80038298 == states);
        assert(D_8003829E == 0 && D_800382CE == expected && alloc_count == 4 && cache_count == 2);
        assert(allocations[1] == expected * 2576U && allocations[2] == expected * 2576U);
        assert(allocations[3] == 664 && D_800382E0[0] == 0 && D_800382E0[1] == 0);
        assert(D_800382D8 == opaque[1] && D_800382DC == opaque[2] && D_800382E4 == opaque[3]);
        for (i = 0; i < counts[a]; i++) {
            assert(states[i].active == 0 && states[i].changed == 0 && states[i].bit_index == 255);
            assert(states[i].unknown01[0] == 0xa5 && states[i].unknown62[5] == 0xa5);
        }
        cases++;
    }
    return cases;
}
static unsigned int test_setup(void)
{
    unsigned short counts[] = {0, 1, 32, 65535};
    unsigned int freqs[] = {0, 44100, 96000};
    unsigned int a, b, mode, scale, i, cases, raw, size;
    unsigned short expected;
    cases = 0; phase = 2;
    for (i = 0; i < 256; i++) D_8002C5D0[i] = (unsigned short)(i);
    D_8002D8C0 = 0.75f; D_8002D8C4 = 1.25f; D_8002D8C8 = 0.5f;
    for (a = 0; a < 4; a++) for (b = 0; b < 3; b++)
    for (mode = 0; mode < 256; mode++) for (scale = 0; scale < 2; scale++) {
        D_80038290 = (unsigned char)scale;
        expected = (unsigned short)(counts[a] * ((unsigned short)(((float)freqs[b] /
            (scale ? 25.0f : 50.0f) * (scale ? D_8002D8C4 : D_8002D8C0)) / 192.0f) + 1));
        raw = (unsigned int)((float)(expected * 784U) * D_8002D8C8);
        size = ((((raw + 63) >> 6) * 40) + 15) & 0xfff0;
        alloc_count = call_count = 0; D_800382CC = 0x7f;
        func_800114C0(counts[a], freqs[b], (unsigned char)mode);
        assert(D_800382A8 == expected && D_800382AC == size && allocations[0] == expected * 24U);
        assert(call_count == 3 && calls[0] == 1 && calls[1] == 2 && calls[2] == 3);
        assert(callback_count == (unsigned short)(D_8002C5D0[mode] * counts[a]));
        cases++;
    }
    return cases;
}
static unsigned int test_submit(void)
{
    unsigned short counts[] = {0, 1, 4, 300};
    unsigned short samples[] = {0, 192, 65535};
    int positions[] = {-193, 0, 192};
    AudioLoop loop;
    unsigned int a, b, c, d, e, i, active, mask, cases;
    unsigned short index;
    void *output;
    void *old_cursor;
    unsigned int native_cursor;
    phase = 3; cases = 0; loop.unknown00 = 0; loop.length = 384;
    for (a = 0; a < 4; a++) for (b = 0; b < 3; b++) for (c = 0; c < 3; c++)
    for (d = 0; d < 2; d++) for (e = 0; e < 2; e++) {
        memset(blocks, 0, sizeof(blocks)); memset(states, 0xa5, sizeof(states));
        D_80038294 = states; D_8003829C = counts[a]; D_800382D0 = blocks;
        index = 0; D_800382D4 = &index; D_800382CE = (unsigned short)(e + 1);
        D_800382E8 = d ? &loop : 0; D_800382EC = positions[c]; D_800382E4 = opaque[0];
        output = opaque[1]; old_cursor = opaque[2]; D_80038030 = D_80038034 = old_cursor;
        blocks[0].count = b == 0 ? 0 : 32;
        blocks[0].commands[0].status = 0x1234; blocks[0].sample = 0x9876;
        blocks[1].count = 27; expected_samples = samples[b]; seen_count = 0; mask = active = 0;
        for (i = 0; i < counts[a]; i++) {
            states[i].active = (unsigned char)(b == 0 || i % 3 != 0);
            states[i].changed = (unsigned char)(i % 2 == 0);
            states[i].bit_index = (unsigned char)(i % 32);
            if (states[i].changed) mask |= 1U << states[i].bit_index;
        }
        func_800139D4(output, samples[b]);
        for (i = 0; i < counts[a]; i++) {
            assert(states[i].changed == 0 && states[i].unknown01[0] == 0xa5);
            if (states[i].active) {
                assert(seen_index[active] == i && seen_active[active] == (unsigned char)active);
                active++;
            }
        }
        assert((unsigned int)seen_count == active && index == 1);
        assert(blocks[0].changed == mask && blocks[0].work == opaque[0] && blocks[0].loop == D_800382E8);
        if (b == 0) assert(blocks[0].commands[0].status == 0 && blocks[0].commands[0].output == output);
        else assert(blocks[0].commands[0].status == 0x1234 && blocks[0].commands[31].output == output);
        if (d) {
            assert(blocks[0].sample == (unsigned short)(positions[c] / 192));
            assert(D_800382EC == (positions[c] + 192 == loop.length ? 0 : positions[c] + 192));
        } else assert(blocks[0].sample == 0x9876 && D_800382EC == positions[c]);
        if (e) {
            native_cursor = (unsigned int)(unsigned long)&blocks[1].commands[0];
            assert(D_80038030 == &blocks[1].commands[0]);
            assert((unsigned long)D_80038034 == (unsigned long)(native_cursor & ~15U));
            assert(blocks[1].count == 0);
        } else assert(D_80038030 == old_cursor && D_80038034 == old_cursor && blocks[1].count == 27);
        cases++;
    }
    return cases;
}
int main(void)
{
    unsigned int a, b, c;
    assert(sizeof(unsigned int) == 4 && sizeof(AudioState) == 104);
    assert(offsetof(AudioState, bit_index) == 96 && offsetof(AudioState, changed) == 97);
    a = test_init(); b = test_setup(); c = test_submit();
    printf("PASS %u initializer, %u setup, %u submit cases; %u total\n", a, b, c, a + b + c);
    return 0;
}
