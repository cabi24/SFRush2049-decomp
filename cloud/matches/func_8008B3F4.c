/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Clamp to 0.0001 then 1/sqrt. Re-sourced 2026-10-04 with its literal written as a
 * literal (own .rodata word at 0x80123884, verified by the splice): the earlier
 * `extern f32 D_80123884` spelling matched this body alone but broke camera_update_d,
 * which inlines this function twice at -O3.
 */
typedef float f32;
f32 sqrtf(f32);
#pragma intrinsic (sqrtf)

f32 func_8008B3F4(f32 value) {
    if (value < 0.0001f) {
        value = 0.0001f;
    }
    return 1.0f / sqrtf(value);
}
