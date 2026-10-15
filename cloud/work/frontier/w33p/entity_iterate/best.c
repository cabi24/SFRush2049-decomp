/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * UNSCORED source candidate, not a match or a coverage claim.
 * entity_iterate @ 0x800C69C0, native extent 224 bytes.
 * Identity: CameraCollision (GC stree.c, 0x80045D5C, 200 bytes),
 * supported by the pinned GC DWARF signature and N64 native calls/literals.
 *
 * Trace camera motion from lookat to ipos, retaining the surface matrix.
 * On a hit, normalize lookat - ipos, then move ipos 0.25 toward lookat.
 * The native function always returns a null surface pointer.
 *
 * Surface is intentionally opaque; no cross-platform structure is copied.
 * The six world-collision parameters and normalization return type are real.
 * Locals follow the GC evidence: surface pointer, float[3][3], float[3].
 * No padding, inline assembly, false prototypes or shaping devices.
 * Keep the existing frontier_vector_normalize group unchanged in blob_unit.
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
        ipos[0] += temp[0] * 0.25f;
        ipos[1] += temp[1] * 0.25f;
        ipos[2] += temp[2] * 0.25f;
    }
    return (Surface *)0;
}
