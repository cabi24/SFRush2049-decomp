/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef struct Vector { float x, y, z; } Vector;
typedef struct SpatialState {
    u8 unknown00[12];
    Vector position;
    Vector velocity;
    Vector forward;
    Vector side;
    Vector up;
    float inverse[12];
    u8 unknown78[12];
    float gain84;
} SpatialState;
extern u8 D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_8001D5C0(SpatialState *);
int func_8001D660(SpatialState *state, const Vector *position, const Vector *velocity,
                  const Vector *forward, const Vector *up, u8 level)
{
    if (D_8002C630) {
        func_80014594();
        state->position = *position;
        state->velocity = *velocity;
        state->forward = *forward;
        state->up.x = -up->x;
        state->up.y = -up->y;
        state->up.z = -up->z;
        func_8001D5C0(state);
        state->gain84 = (float)level / 127.0f;
        func_800145DC();
        return 1;
    }
    return 0;
}
