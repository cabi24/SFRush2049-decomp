/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * entity_iterate @ 0x800C69C0, 224 bytes (56 words) -- MATCH (score.py fn standalone; unit EQUAL).
 * Identity: GameCube CameraCollision (stree.c, 0x80045D5C), proposed rename (not applied).
 * No arcade ancestor in reference/repos/rushtherock.
 *
 * Trace the camera from lookat to ipos through the world collision
 * (input_deadzone_apply = GC ShotToWorldCollision(old, new, uvs, radius, interp, ignore)).
 * On a hit, normalise lookat - ipos (func_8008E0B8 = GC NormalVector) and move ipos a
 * quarter of the unit vector toward lookat. Always returns a null surface pointer.
 *
 * Lever (w33c): the update is written in GC's fmadds operand form,
 * `ipos[k] = ipos[k] + 0.25f * temp[k]` -- uopt keeps the product as the first add operand, so
 * ugen evaluates temp*0.25 before the ipos load (retail's FP temp order). `+=` and
 * `temp * 0.25f + ipos` both left ipos first (15/56, w33p). Locals as GC: surface pointer,
 * float[3][3], float[3] (frame 88 exact). No shaping devices, pads or false prototypes.
 */
typedef struct Surface Surface;

extern Surface *input_deadzone_apply(float *old, float *newpos,
    float (*uvs)[3], float radius, int interp, int ignore);
extern float func_8008E0B8(float *vector);

Surface *entity_iterate(float *ipos, float *lookat)
{
    Surface *surfaceptr;
    float surfuvs[3][3];
    float temp[3];

    surfaceptr = input_deadzone_apply(lookat, ipos, surfuvs, 0.5f, 1, 6);
    if (surfaceptr) {
        temp[0] = lookat[0] - ipos[0];
        temp[1] = lookat[1] - ipos[1];
        temp[2] = lookat[2] - ipos[2];
        func_8008E0B8(temp);
        ipos[0] = ipos[0] + 0.25f * temp[0];
        ipos[1] = ipos[1] + 0.25f * temp[1];
        ipos[2] = ipos[2] + 0.25f * temp[2];
    }
    return (Surface *)0;
}
