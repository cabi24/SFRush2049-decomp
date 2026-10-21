/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Complete vector length and guarded in-place normalization.
 * E098 retains its existing proven three-float body and public entry point.
 * The ordinary whole-program O3 pipeline inlines that real body into E0B8.
 * IDO's sqrtf intrinsic supplies the native single-precision square root.
 * No exact arcade donor or historical source spelling is established.
 */
typedef float f32;

f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

f32 func_8008E098(f32 x, f32 y, f32 z)
{
    return sqrtf((x * x + y * y) + z * z);
}

extern f32 D_8012394C;

f32 func_8008E0B8(f32 *v)
{
    f32 length, inverse_length;

    length = func_8008E098(v[0], v[1], v[2]);
    if (length <= D_8012394C) {
        return 0.0f;
    }
    inverse_length = 1.0f / length;
    v[0] *= inverse_length;
    v[1] *= inverse_length;
    v[2] *= inverse_length;
    return length;
}
