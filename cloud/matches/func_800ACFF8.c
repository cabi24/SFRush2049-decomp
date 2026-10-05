/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Rotate rows 1 and 2 of an in-place 3x3 matrix using supplied sine/cosine.
 * The temporary preserves the first row component until both outputs are ready.
 * The written addition order matters to IDO's multiplication evaluation order.
 * No padding locals, fake arguments, inline assembly or auxiliary context.
 * Derived from the complete native body; no arcade ancestor is asserted.
 */
typedef float f32;
void func_800ACFF8(f32 sine, f32 cosine, f32 m[3][3]) {
    f32 result;
    result = m[1][0] * cosine - m[2][0] * sine;
    m[2][0] = m[2][0] * cosine + m[1][0] * sine;
    m[1][0] = result;
    result = m[1][1] * cosine - m[2][1] * sine;
    m[2][1] = m[2][1] * cosine + m[1][1] * sine;
    m[1][1] = result;
    result = m[1][2] * cosine - m[2][2] * sine;
    m[2][2] = m[2][2] * cosine + m[1][2] * sine;
    m[1][2] = result;
}
