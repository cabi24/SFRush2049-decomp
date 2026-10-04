/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct Vector { float x, y, z; } Vector;
typedef struct SpatialState {
    unsigned char unknown00[12];
    Vector position;
    unsigned char unknown18[12];
    Vector forward;
    Vector side;
    Vector up;
    float inverse[12];
} SpatialState;
extern void func_80024D04(Vector *, const Vector *, const Vector *);
extern void func_80024D74(float *, const float *);
void func_8001D5C0(SpatialState *state)
{
    float matrix[12];
    func_80024D04(&state->side, &state->up, &state->forward);
    matrix[0] = state->side.x;
    matrix[3] = state->side.y;
    matrix[6] = state->side.z;
    matrix[1] = state->up.x;
    matrix[4] = state->up.y;
    matrix[7] = state->up.z;
    matrix[2] = state->forward.x;
    matrix[5] = state->forward.y;
    matrix[8] = state->forward.z;
    matrix[9] = state->position.x;
    matrix[10] = state->position.y;
    matrix[11] = state->position.z;
    func_80024D74(state->inverse, matrix);
}
