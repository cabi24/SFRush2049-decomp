/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed short s16;
typedef int s32;
typedef float f32;
typedef struct Coefficients {u8 pad0[160]; f32 rate; u8 pad164[4]; f32 gain;} Coefficients;
typedef struct Gain {u8 pad0[16]; f32 factor;} Gain;
typedef struct State {Coefficients *coefficients; Gain *gain; u8 pad8[964]; f32 position; u8 pad976[36]; s16 enabled; u8 pad1014[14]; f32 drive, value, spare1036, output, ratio, effective, target, force, goal, spare1064, omega; u8 pad1072[516]; f32 dt;} State;
extern f32 D_801243D8;
extern f32 D_801243DC;
void func_800E2AC4(State *state) {
    f32 omega, square, limit, force, target, drive, goal, value, current;
    s32 below, above;
    current = state->value;
    omega = state->omega;
    square = omega * omega;
    if (state->value < D_801243D8) state->value = D_801243D8;
    if (D_801243DC < state->position) force = 0.0f;
    else force = (D_801243DC - state->position) * 1.25f * state->coefficients->gain;
    if (!state->enabled || force == 0.0f) {
        state->force = 0.0f;
        state->output = 0.0f;
        state->goal = state->value;
        state->effective = state->ratio;
        state->value += state->drive * state->coefficients->rate * state->dt;
        return;
    }
    below = 0;
    force *= state->gain->factor;
    target = state->target * state->omega;
    state->goal = target;
    if (target < state->value) below = 1;
    if (below) state->force = force;
    else state->force = -force;
    drive = state->drive;
    goal = state->goal;
    above = 0;
    value = state->value + (drive - state->force) * state->coefficients->rate * state->dt;
    if (goal < value) above = 1;
    if (above != below) {state->value = goal; state->force = drive;}
    else state->value = value;
    state->output = state->force * state->omega;
    state->effective = 1.0f / (1.0f / state->ratio + square / state->coefficients->rate);
}
