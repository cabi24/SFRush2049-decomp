/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001D764 - lib_1cf90 (IDO 7.1). Register a spatial (3D) listener/emitter state: with the
 * audio system up, inside func_80014594/func_800145DC, push STATE on the front of the list
 * D_8004FD54, copy position/velocity/forward, store the negated UP vector, the three
 * parameters (0x78/0x7C/0x80), rebuild its basis with func_8001D5C0 (locked, same TU), set FLAGS
 * and gain LEVEL / 127. Returns 1, or 0 when audio is down.
 * Base: w17g lockonly/func_8001D764.c (D10 body; LOCK-exact under 7.1, TU-blocked by its
 * `extern SpatialState *D_8004FD54` against the TU's `extern LinkNode *D_8004FD54`).
 * w18c: the state view is private (SpatialState_8001D764, D10's fields) with next/previous
 * typed LinkNode * (boot_tail_sample_contract.h, the TU's include), D_8004FD54 is the TU's
 * LinkNode * and func_8001D5C0 keeps the TU's SpatialState prototype: the pointer casts
 * `(LinkNode *)state` / `(SpatialState *)state` are type views only (no code). The TU's
 * SpatialState typedef (func_8001D5C0's block) is copied verbatim for that prototype. */
#include "boot_tail_sample_contract.h"
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct Vector { float x, y, z; } Vector;
typedef struct SpatialState { unsigned char unknown00[12]; Vector position; unsigned char unknown18[12]; Vector forward; Vector side; Vector up; float inverse[12]; } SpatialState;
typedef struct SpatialState_8001D764 {
    LinkNode *next, *previous;
    u32 flags08;
    Vector position, velocity, forward, side, up;
    float inverse[12];
    float first78, second7C, third80, gain84;
} SpatialState_8001D764;
extern u8 D_8002C630;
extern LinkNode *D_8004FD54;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_8001D5C0(SpatialState *);

int func_8001D764(SpatialState_8001D764 *state, const Vector *position, const Vector *velocity,
                  const Vector *forward, const Vector *up, float first, float second,
                  float third, u32 flags, u8 level)
{
    if (D_8002C630) {
        func_80014594();
        if ((state->next = D_8004FD54) != 0) D_8004FD54->previous = (LinkNode *)state;
        state->previous = 0;
        D_8004FD54 = (LinkNode *)state;
        state->position = *position;
        state->velocity = *velocity;
        state->forward = *forward;
        state->up.x = -up->x;
        state->up.y = -up->y;
        state->up.z = -up->z;
        state->first78 = first;
        state->second7C = second;
        state->third80 = third;
        func_8001D5C0((SpatialState *)state);
        state->flags08 = flags;
        state->gain84 = (float)level / 127.0f;
        func_800145DC();
        return 1;
    }
    return 0;
}
