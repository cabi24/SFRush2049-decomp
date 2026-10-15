/* Review-only N64 seed derived from the native 224-byte body at 0x800C69C0.
 * Historical target name: entity_iterate. Proposed identity: CameraCollision.
 * Not compiled, scored, linked, or applied. The current locked normalization
 * group must remain in the whole-program unit when this is scored.
 */
typedef struct Surface Surface;
extern Surface *input_deadzone_apply(float *old, float *newpos,
    float (*uvs)[3], float radius, int interp, int ignore);
extern float func_8008E0B8(float *vector);

Surface *entity_iterate(float *ipos, float *lookat)
{
    float surfuvs[3][3];
    float temp[3];

    if (input_deadzone_apply(lookat, ipos, surfuvs, 0.5f, 1, 6)) {
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
