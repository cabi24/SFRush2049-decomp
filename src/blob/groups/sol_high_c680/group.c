/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Match recipe: compile as a genuine call group, keep func_8008C680 external,
 * leave func_8008C5E0 internal via uld -kp. See group.json and STATUS.md.
 * C680: piecewise argument reduction around a rational approximation.
 * C5E0: real sole callee; reconstructed from locked src/blob/func_8008C5E0.c.
 * No coefficient values are guessed; all are the original external f32 data.
 */
typedef float f32;
extern f32 D_801238C0;
extern f32 D_801238C4;
extern f32 D_801238C8;
extern f32 D_801238CC;
extern f32 D_801238D0;
extern f32 D_801238D4;
extern f32 D_801238D8;
extern f32 D_801238DC;
extern f32 D_801238E0;
extern f32 D_801238E4;
extern f32 D_801238E8;
extern f32 D_801238EC;
extern f32 D_801238F0;

f32 func_8008C5E0(f32 arg0)
{
  f32 square;
  f32 constant;
  square = arg0 * arg0;
  constant = D_801238C0;
  return (((((((((D_801238C4 * square) + D_801238C8) * square) + D_801238CC) * square) + D_801238D0) * square) + constant) / (((((((((square + D_801238D4) * square) + D_801238D8) * square) + D_801238DC) * square) + D_801238E0) * square) + constant)) * arg0;
}

f32 func_8008C680(f32 arg0) {
    if (arg0 < D_801238E4) {
        return func_8008C5E0(arg0);
    }
    if (D_801238E8 < arg0) {
        return D_801238EC - func_8008C5E0(1.0f / arg0);
    }
    return func_8008C5E0((arg0 - 1.0f) / (arg0 + 1.0f)) + D_801238F0;
}
