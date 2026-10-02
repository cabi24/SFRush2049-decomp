/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed short s16;
typedef int s32;
typedef float f32;
typedef struct State {u8 pad0[748]; f32 matrix[9]; u8 pad784[472]; f32 factor1256; u8 pad1260[68]; f32 factor1328; u8 pad1332[16]; f32 factor1348; u8 pad1352[68]; f32 factor1420; u8 pad1424[60]; f32 inputA[4]; u8 pad1500[16]; f32 inputB[4]; u8 pad1532[312]; f32 inputC[9]; s16 packed; u8 pad1882[2]; f32 outputA[4], outputB[4], outputC[9], outputMatrix[9];} State;
extern f32 D_801241A4;
extern void math_utility(f32 *input, f32 *output);
void func_800D4DFC(State *state) {
    s32 packed, i;
    math_utility(state->matrix, state->outputMatrix);
    packed = (state->factor1348 * state->factor1420 + state->factor1328 * state->factor1256) * 0.5f * D_801241A4;
    for (i = 0; i < 4; i++) state->outputA[i] = state->inputA[i];
    for (i = 0; i < 4; i++) state->outputB[i] = state->inputB[i];
    for (i = 0; i < 9; i++) state->outputC[i] = state->inputC[i];
    state->packed = packed;
}
