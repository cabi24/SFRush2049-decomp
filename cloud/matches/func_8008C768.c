/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* single file; -O2 leaves 22 words: y in $f20 instead of $f16 */
typedef float f32;

extern f32 func_8008C680(f32);
extern f32 func_8008C720(f32);
extern const f32 D_801238F4;
extern const f32 D_801238F8;
extern const f32 D_801238FC;
extern const f32 D_80123900;

f32 func_8008C768(f32 y, f32 x) {
    if (y == y + x) {
        if (y == 0.0f) {
            return 0;
        }
        if (y > 0.0f) {
            return D_801238F4;
        }
        return D_801238F8;
    }
    if (x < 0.0f) {
        if (y >= 0.0f) {
            return D_801238FC - func_8008C680(-y / x);
        }
        return func_8008C680(y / x) + D_80123900;
    }
    if (y > 0) {
        return func_8008C720(y / x);
    }
    return -func_8008C720(-y / x);
}
