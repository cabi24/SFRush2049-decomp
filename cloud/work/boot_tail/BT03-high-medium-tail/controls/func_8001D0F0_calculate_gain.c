/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef struct Vector { float x, y, z; } Vector;
typedef struct Emitter {
    u8 unknown00[12];
    Vector position;
    Vector velocity;
    unsigned int unknown24;
    float gain28;
    float current2C;
} Emitter;
extern u8 D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
int func_8001D0F0(Emitter *state, const Vector *position, const Vector *velocity, u8 level)
{
    float gain;
    if (D_8002C630) {
        func_80014594();
        gain = (float)level / 127.0f;
        state->position = *position;
        state->velocity = *velocity;
        state->gain28 = gain;
        if (state->gain28 < state->current2C) state->current2C = state->gain28;
        func_800145DC();
        return 1;
    }
    return 0;
}
