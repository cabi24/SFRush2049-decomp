/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001D0F0 - lib_1cf90 (IDO 7.1). Update a positional sound emitter: with the audio
 * system up (D_8002C630), inside func_80014594/func_800145DC (lock/unlock), copy POSITION and
 * VELOCITY into the emitter, set its target gain to LEVEL / 127 and clamp the current gain
 * down to it. Returns 1, or 0 when audio is down.
 * Base: w17g lockonly/func_8001D0F0.c (D10 body; LOCK-exact under 7.1, TU-blocked: the TU's
 * Vector typedef sits after this slot). w18c: Vector is the TU's (func_8001D5C0's block,
 * verbatim), hoisted above this slot by a TU edit (RESULTS.md "TU edits"); the emitter view is
 * private (Emitter_8001D0F0, D10's fields). No shaping devices. */
typedef unsigned char u8;
typedef struct Vector { float x, y, z; } Vector;
typedef struct Emitter_8001D0F0 {
    u8 unknown00[12];
    Vector position;
    Vector velocity;
    unsigned int unknown24;
    float gain28;
    float current2C;
} Emitter_8001D0F0;
extern u8 D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);

int func_8001D0F0(Emitter_8001D0F0 *state, const Vector *position, const Vector *velocity, u8 level)
{
    if (D_8002C630) {
        func_80014594();
        state->position = *position;
        state->velocity = *velocity;
        state->gain28 = (float)level / 127.0f;
        if (state->gain28 < state->current2C) state->current2C = state->gain28;
        func_800145DC();
        return 1;
    }
    return 0;
}
