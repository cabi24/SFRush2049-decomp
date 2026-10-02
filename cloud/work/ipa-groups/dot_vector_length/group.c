/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Complete three-component vector length at 0x8008E098.
 * Arcade equivalent unknown: the primary arcade reference is unavailable here.
 * IDO's standard sqrtf intrinsic emits the native single-precision square root.
 * Compile through the whole-program O3 group pipeline; no caller context needed.
 */
typedef float f32;

f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

f32 func_8008E098(f32 x, f32 y, f32 z)
{
    return sqrtf((x * x + y * y) + z * z);
}
