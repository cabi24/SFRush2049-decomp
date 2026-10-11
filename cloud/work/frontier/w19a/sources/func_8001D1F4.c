/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001D1F4 - lib_1cf90 (IDO 7.1). Set up a 3D sound emitter: under the audio lock
 * (func_80014594/func_800145DC) fill STATE (or the shared one-shot record D_8004FD58 when
 * STATE is null) with flags, position, velocity, range, sound id, gain and minimum (u8 / 127)
 * and curve and CONTEXT. One-shot (null STATE): evaluate it (func_8001C860 -> volume, pitch,
 * pan, span, send); silent -> -1; else start the sound (func_80020174(id, 127, 64)) and, if it
 * started, apply the levels (func_8001CCDC) and return its handle. Persistent STATE: link it at
 * the head of the emitter list D_8004FD50, no handle yet (-1), counter 0, flags |= 0x30000;
 * returns -1. Callers: func_8001D3F0, func_8001D460 (lib_1cf90, locked).
 * Types: the TU's declarations: prototype as declared after the slot (func_8001D3F0's block:
 * `int`, `void *` / `const void *` record pointers), D_8004FD50 `StateNode *` and StateNode
 * from boot_tail_sample_contract.h, func_8001CCDC's definition, Vector as the TU's typedef.
 * The emitter record is a private full view (Emitter_8001D1F4, D10's Emitter) whose list links
 * are StateNode pointers (the list D_8004FD50 holds); `target` is converted with casts where it
 * enters the list / func_8001C860 / func_8001CCDC (type views only; a cast on D_8004FD50
 * itself changes the code, true=280). func_8001C860 (unmatched): the TU's prototype
 * (func_8001DDE0's block, after this slot).
 * w19a: from D10 BT03-high-spatial-control (5.3; 7.1 true=0 strict=207, frame 64 vs 72):
 * the five outputs declared in retail's slot order (pan, span, send, volume, pitch) and the
 * started handle kept in a real local `handle` (retail's 72-byte frame). No shaping devices,
 * no unused locals. */
#include "types.h"
#include "boot_tail_sample_contract.h"
typedef struct Vector { float x, y, z; } Vector;
typedef struct Emitter_8001D1F4 {
    StateNode *next, *previous;
    u32 flags;
    Vector position, velocity;
    float range, gain, minimum, curve;
    u32 handle, context;
    u16 identifier, counter;
    float fade;
} Emitter_8001D1F4;
extern StateNode *D_8004FD50;
extern Emitter_8001D1F4 D_8004FD58;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_8001C860(StateNode *, float *, float *, float *, float *, float *);
extern void func_8001CCDC(StateNode *em, float vol, float xPan, float yPan, float zPan, float doppler);
extern u32 func_80020174(u16, u8, u8);
int func_8001D1F4(void *state, const void *position, const void *velocity, float range, float curve,
                  u32 flags, u16 identifier, u32 context, u8 gain, u8 minimum)
{
    Emitter_8001D1F4 *target;
    float pan, span, send, volume, pitch;
    u32 handle;
    func_80014594();
    if (state == 0) target = &D_8004FD58;
    else target = state;
    target->flags = flags;
    target->position = *(const Vector *)position;
    target->velocity = *(const Vector *)velocity;
    target->range = range;
    target->identifier = identifier;
    target->gain = (float)gain / 127.0f;
    target->minimum = (float)minimum / 127.0f;
    target->curve = curve;
    target->context = context;
    if (state == 0) {
        func_8001C860((StateNode *)target, &volume, &pitch, &pan, &span, &send);
        if (volume == 0.0f) {
            func_800145DC();
            return -1;
        }
        handle = func_80020174(target->identifier, 127, 64);
        target->handle = handle;
        if (handle == 0xFFFFFFFF) {
            func_800145DC();
            return -1;
        }
        func_8001CCDC((StateNode *)target, volume, pan, span, send, pitch);
        func_800145DC();
        return target->handle;
    }
    if ((target->next = D_8004FD50) != 0) D_8004FD50->previous = (StateNode *)target;
    target->previous = 0;
    D_8004FD50 = (StateNode *)target;
    target->handle = 0xFFFFFFFF;
    target->counter = 0;
    target->flags |= 0x30000;
    func_800145DC();
    return -1;
}
