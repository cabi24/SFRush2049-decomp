/* Bounded source fixtures; the actual external allocator is represented by a hook. */
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "group.c"
struct Pool { s32 marker; };
Pool D_80395EE8;
static Effect output, expected;
static s32 calls, fail_alloc;
static Effect *answer;
Effect *func_8008E3C0(Pool *pool) {
    if (pool != &D_80395EE8) abort();
    calls++;
    return fail_alloc ? 0 : answer;
}
static f32 *select_position(Effect *e, s32 which, f32 *separate) {
    switch(which) {
    case 0: return separate;
    case 1: return e->origin;
    case 2: return e->previous;
    case 3: return e->lower;
    default: return e->upper;
    }
}
static void oracle(Effect *e, s32 mode, f32 *p) {
    f32 amount;
    e->lifetime = 0.1f;
    amount = mode == 0 ? 0.75f : mode == 1 ? 0.5f : mode == 2 ? 0.35f : 0.025f;
    e->kind = 1U << mode;
    if (mode == 3) e->lifetime = 0.0f;
    e->origin[0] = p[0]; e->origin[1] = p[1]; e->origin[2] = p[2];
    e->previous[0] = p[0]; e->previous[1] = p[1]; e->previous[2] = p[2];
    e->lower[0] = p[0]; e->lower[1] = p[1]; e->lower[2] = p[2];
    e->upper[0] = p[0]; e->upper[1] = p[1]; e->upper[2] = p[2];
    e->lower[1] = e->lower[1] - amount;
    e->upper[1] = e->upper[1] + amount;
}
int main(void) {
    s32 mode, alias, seed, failure, i, count = 0;
    f32 pos[3], model_pos[3];
    Effect *actual;
    for (mode=0; mode<4; mode++) for (alias=0; alias<5; alias++)
    for (seed=0; seed<6; seed++) for (failure=0; failure<2; failure++) {
        memset(&output, 0, sizeof(output));
        output.next = &expected;
        output.unknown04 = 0xfeed0000U + (u32)seed;
        output.kind = 99;
        output.lifetime = -4.0f;
        for (i=0; i<3; i++) {
            pos[i] = (f32)(seed * 7 - i * 2 - 10) / 4.0f;
            output.origin[i] = (f32)(seed + i);
            output.previous[i] = (f32)(seed - i);
            output.lower[i] = (f32)(seed * 2 + i);
            output.upper[i] = (f32)(seed * 3 - i);
        }
        memcpy(model_pos, pos, sizeof(pos));
        expected = output;
        fail_alloc=failure; calls=0; answer=&output;
        if (!failure) oracle(&expected, mode, select_position(&expected,alias,model_pos));
        actual=func_8038D054(mode,select_position(&output,alias,pos),(f32 *)0);
        if (actual != (failure ? 0 : &output) || calls != 1 ||
            memcmp(&output,&expected,sizeof(output)) || memcmp(pos,model_pos,sizeof(pos))) abort();
        count++;
    }
    if (count != 240) abort();
    puts("PASS: 240 source fixtures including null allocation and overlapping positions");
    return 0;
}
