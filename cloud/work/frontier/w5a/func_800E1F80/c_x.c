typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;

typedef struct Push {
    u16 pad0;
    u16 flags;              /* +2: bits 11..15 id/strength, 0x10, 0x20 */
    s16 dir[3];             /* +4 */
} Push;

typedef struct Surface {
    u16 pad0;
    u16 flags;              /* +2 */
    u8 pad4[20];
} Surface;

typedef struct Car {
    u8 pad0[100];
    f32 wheelForce[4][3];   /* +100 */
    u8 pad148[144];
    f32 force[3];           /* +292 */
    u8 pad304[240];
    f32 uv[3];              /* +544 */
    f32 torque[3];          /* +556 */
    u8 pad568[180];
    f32 bodyUV[9];          /* +748 */
    u8 pad784[656];
    u16 surface[4];         /* +1440 */
    u8 pad1448[24];
    f32 mass;               /* +1472 */
    u8 pad1476[40];
    f32 tireLoad[4];        /* +1516 */
    u8 pad1532[56];
    f32 scale;              /* +1588 */
    u8 pad1592[16];
    Push *push;             /* +1608 */
} Car;

extern Surface *D_801497F8;
extern void func_800A61B0(f32 *in, f32 *out, f32 *uv);
extern void func_800C1A00(s32 id, f32 *out);
extern f32 func_8008E0B8(f32 *v);

void func_800E1F80(Car *m) {
    Push *p;
    s32 i;
    f32 v[3];
    f32 out[3];
    s32 count;
    s32 mag;
    u32 mask;
    f32 d;

    p = m->push;
    count = 0;
    mask = 0;
    i = 0;
    mag = (p->flags & 0xF800) >> 11;
    if (p->flags & 0x10) {
        mag *= 8;
        if (mag > 50) {
            v[0] = p->dir[0] * (1.0f / 16384.0f);
            v[1] = p->dir[1] * (1.0f / 16384.0f);
            v[2] = p->dir[2] * (1.0f / 16384.0f);
            d = v[0] * m->uv[0] + v[1] * m->uv[1] + v[2] * m->uv[2];
            d = (mag * 1.4666667f - d) * m->mass;
            out[0] = v[0] * d;
            out[1] = v[1] * d;
            out[2] = v[2] * d;
            func_800A61B0(out, v, m->bodyUV);
            m->force[0] += v[0];
            m->force[1] += v[1];
            return;
        }
    }
    for (i = 0; i < 4; i++) {
        if (m->tireLoad[i] <= 0.0f) {
            if (D_801497F8[m->surface[i]].flags & 0x30) {
                count++;
                mask |= 1 << i;
            }
        }
    }
    if (count >= 3) {
        if (p->flags & 0x20) {
            func_800C1A00(mag, v);
            m->torque[0] += v[0] * m->scale;
            m->torque[2] += v[2] * m->scale;
        } else {
            d = mag * 1.4666667f * m->scale;
            m->torque[0] += d * p->dir[0] * (1.0f / 16384.0f);
            m->torque[2] += d * p->dir[2] * (1.0f / 16384.0f);
        }
        return;
    }
    for (i = 0; i < 4; i++) {
        if ((1 << i) & mask) {
            if (p->flags & 0x20) {
                func_800C1A00(mag, v);
                func_8008E0B8(v);
            } else {
                v[0] = p->dir[0] * (1.0f / 16384.0f);
                v[1] = p->dir[1] * (1.0f / 16384.0f);
                v[2] = p->dir[2] * (1.0f / 16384.0f);
            }
            v[0] *= 30.0f * m->mass;
            v[1] *= 30.0f * m->mass;
            v[2] *= 30.0f * m->mass;
            func_800A61B0(v, out, m->bodyUV);
            m->wheelForce[i][0] += out[0];
            m->wheelForce[i][1] += out[1];
            m->wheelForce[i][2] += out[2];
        }
    }
}
