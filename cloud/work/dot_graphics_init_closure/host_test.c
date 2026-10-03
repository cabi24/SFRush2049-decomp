/* Contract tests: mode helper is an event-logging stub. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
typedef unsigned int u32;
typedef int s32;
typedef struct { u32 w0, w1; } Gfx;
Gfx *D_80149438;
Gfx D_80124FE8[2][2400];
u32 D_8012E608;
s32 D_8012E60C, D_8012E668, D_8012E610;
s32 D_8002AFC0, D_8002AFC4, D_8012E674;
short D_8012E67A;
unsigned char D_8011EACF;
s32 D_8012E684, D_8012E6C0, D_8014A248, D_8012E680, D_8012E6D0;
long long D_8012E688;
static int calls, modes[3];
void func_80086A50(s32 mode) { assert(calls < 3); modes[calls++] = mode; }
extern void sound_init(void);
extern void func_800878E0(u32);

static void initialization(int bank, int height) {
    int i;
    Gfx *g;
    D_80149438 = NULL; D_8012E6C0 = bank;
    D_8002AFC0 = 320; D_8002AFC4 = height;
    D_8012E608 = D_8012E60C = D_8012E668 = 123;
    D_8012E67A = 123; D_8012E684 = 123; D_8012E688 = 123;
    D_8012E6D0 = 123; calls = 0;
    memset(D_80124FE8, 0xAB, sizeof(D_80124FE8));
    sound_init();
    assert(D_8012E6C0 == (bank+1 >= 2 ? 0 : bank+1));
    g = D_80124FE8[D_8012E6C0];
    assert(D_80149438 == g+4);
    assert(g[0].w0 == 0xF9000000 && g[0].w1 == 16);
    assert(g[1].w0 == 0xE7000000 && g[1].w1 == 0);
    assert(g[2].w0 == 0xE3000C00 && g[2].w1 == 0);
    assert(g[3].w0 == 0xD9000000 && g[3].w1 == 0);
    for (i=4;i<2400;i++) assert(g[i].w0 == 0xABABABAB && g[i].w1 == 0xABABABAB);
    assert(D_8012E608 == (height >= 221 ? 0x8000 : 0));
    assert(D_8012E60C == 0 && D_8012E668 == 0);
    assert(D_8012E610 == 319 && D_8012E674 == height-1);
    assert(D_8012E67A == 0 && D_8011EACF == 255);
    assert(D_8012E684 == 0 && D_8012E688 == 0 && D_8012E6D0 == 0);
    assert(D_8014A248 == -1 && D_8012E680 == -1);
    assert(calls == 1 && modes[0] == 1);
    /* A non-null cursor prevents all work. */
    D_8012E60C = 77; D_8012E688 = 99; calls = 0;
    sound_init();
    assert(D_80149438 == g+4 && D_8012E60C == 77 && D_8012E688 == 99 && calls == 0);
}

static void flags_test(u32 prior, u32 flags, int mode) {
    Gfx *g = D_80124FE8[0];
    int n=0, expected_calls=0, expected_mode=mode;
    D_80149438 = g; D_8012E608 = prior; D_8014A248 = mode; calls=0;
    memset(g, 0xAB, 32*sizeof(*g));
    func_800878E0(flags);
    if ((prior & flags) != flags) {
        assert((u32)D_8012E608 == (prior | flags));
        if (flags & 0x4000) {
            assert(g[n].w0 == 0xFCFFFFFF && g[n].w1 == 0xFFFDF6FB); n++;
            if (mode <= 0 || mode >= 4) {
                assert(g[n].w0 == 0xE3000A01 && g[n].w1 == 0); n++;
            }
            expected_mode = -1;
        }
        if (flags & 1) { assert(g[n].w0 == 0xE2001E01 && g[n].w1 == 1); n++; }
        if (flags & 0x10) { assert(g[n].w0 == 0xE2001D00 && g[n].w1 == 4); n++; expected_calls++; }
        if (flags & 0x20) expected_calls++;
    } else assert((u32)D_8012E608 == prior);
    assert(D_80149438 == g+n && D_8014A248 == expected_mode && calls == expected_calls);
    while (expected_calls) { expected_calls--; assert(modes[expected_calls] == expected_mode); }
    assert(g[n].w0 == 0xABABABAB && g[n].w1 == 0xABABABAB);
}
int main(void) {
    unsigned int i, j, state=12345;
    int mode;
    for (i=0;i<2;i++) for (j=0;j<5;j++) initialization(i, 219+j);
    for (i=0;i<64;i++) for(j=0;j<64;j++) for(mode=-1;mode<=5;mode++)
        flags_test((i & 31) | ((i & 32) ? 0x4000 : 0), (j & 31) | ((j & 32) ? 0x4000 : 0), mode);
    for (i=0;i<10000;i++) {
        u32 prior, flags;
        state=state*1664525u+1013904223u; prior=state;
        state=state*1664525u+1013904223u; flags=state;
        flags_test(prior, flags, (int)(i%7)-1);
    }
    puts("PASS: 10 initialization/idempotence + 28672 flag grid + 10000 full-width cases");
    return 0;
}
