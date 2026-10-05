/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Rotate rows 0 and 2 of a 3x3 matrix using supplied sine and cosine.
 * The one result temporary preserves the original row-0 component until
 * both actual outputs have been evaluated. Written sum operand order
 * selects the native IDO multiplication order, as in the row-rotation pair.
 * Complete ordinary three-input ABI; no artificial locals or context.
 * Derived from the complete native body; no arcade ancestor is asserted.
 */
typedef float f32;
void func_800C40E8(f32 sine, f32 cosine, f32 m[3][3]) {
    f32 result;
    result = m[0][0] * cosine + m[2][0] * sine;
    m[2][0] = m[2][0] * cosine - m[0][0] * sine;
    m[0][0] = result;
    result = m[0][1] * cosine + m[2][1] * sine;
    m[2][1] = m[2][1] * cosine - m[0][1] * sine;
    m[0][1] = result;
    result = m[0][2] * cosine + m[2][2] * sine;
    m[2][2] = m[2][2] * cosine - m[0][2] * sine;
    m[0][2] = result;
}
