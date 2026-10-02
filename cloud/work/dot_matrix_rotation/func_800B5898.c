/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* NONMATCH: 5/42 full words differ; no accepted coverage.
 * Rotate the Y/Z pair of each of three matrix rows when angle is outside
 * the native deadband. Arcade equivalent is not established (not available
 * in this checkout). All locals carry actual values; no frame padding.
 */
typedef float f32;
typedef int s32;
extern f32 D_80123DB4, D_80123DB8;
f32 sinf(f32);
f32 cosf(f32);
void func_800B5898(f32 angle, f32 *matrix) {
    f32 cosine;
    f32 sine;
    f32 z;
    f32 y;
    f32 y_cosine;
    s32 row;
    f32 *cursor;

    if ((angle < D_80123DB4) || (D_80123DB8 < angle)) {
        sine = sinf(angle);
        cosine = cosf(angle);
        row = 0;
        cursor = matrix;
        do {
            y = cursor[1];
            z = cursor[2];
            row += 1;
            y_cosine = y * cosine;
            cursor += 3;
            cursor[-2] = (f32) (y_cosine - (z * sine));
            cursor[-1] = (f32) ((y * sine) + (z * cosine));
        } while (row != 3);
    }
}
