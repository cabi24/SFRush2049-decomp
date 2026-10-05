typedef float f32;
typedef signed char s8;
typedef unsigned char u8;
typedef int s32;

typedef struct {
    u8 col[3];
    char pad1;
    u8 colc[3];
    char pad2;
    s8 dir[3];
    char pad3;
    char pad4[4];
} Light_t;

typedef struct {
    Light_t l[2];
} LookAt;

extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
extern f32 D_80123AEC, D_80123AF0, D_80123AF4, D_80123AF8;

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

void camera_update_d(LookAt *l, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt,
                     f32 xUp, f32 yUp, f32 zUp) {
    f32 len, xR, yR, zR, xLook, yLook, zLook;
    f32 xRight, yRight, zRight;
    f32 p0;
    f32 lensq;
    f32 t;

    xLook = xAt - xEye;
    yLook = yAt - yEye;
    zLook = zAt - zEye;
    lensq = xLook * xLook + yLook * yLook + zLook * zLook;
    if (lensq < D_80123AEC) {
        lensq = D_80123AF0;
    }
    len = -(1.0f / sqrtf(lensq));
    xLook *= len;
    yLook *= len;
    zLook *= len;

    xR = yUp * zLook - zUp * yLook;
    yR = zUp * xLook - xUp * zLook;
    zR = xUp * yLook - yUp * xLook;
    lensq = xR * xR + yR * yR + zR * zR;
    if (lensq < D_80123AF4) {
        lensq = D_80123AF8;
    }
    len = 1.0f / sqrtf(lensq);
    xRight = xR * len;
    yRight = yR * len;
    zRight = zR * len;

    t = xRight * 128.0f;
    l->l[0].dir[0] = (s32) MIN(t, 127.0f);
    t = yRight * 128.0f;
    l->l[0].dir[1] = (s32) MIN(t, 127.0f);
    t = zRight * 128.0f;
    l->l[0].dir[2] = (s32) MIN(t, 127.0f);
    t = (yLook * zRight - zLook * yRight) * 128.0f;
    l->l[1].dir[0] = (s32) MIN(t, 127.0f);
    t = (zLook * xRight - xLook * zRight) * 128.0f;
    l->l[1].dir[1] = (s32) MIN(t, 127.0f);
    t = (xLook * yRight - yLook * xRight) * 128.0f;
    l->l[1].dir[2] = (s32) MIN(t, 127.0f);

    l->l[0].col[0] = 0;
    l->l[0].col[1] = 0;
    l->l[0].col[2] = 0;
    l->l[0].pad1 = 0;
    l->l[0].colc[0] = 0;
    l->l[0].colc[1] = 0;
    l->l[0].colc[2] = 0;
    l->l[0].pad2 = 0;
    l->l[1].col[0] = 0;
    l->l[1].col[1] = 0x80;
    l->l[1].col[2] = 0;
    l->l[1].pad1 = 0;
    l->l[1].colc[0] = 0;
    l->l[1].colc[1] = 0x80;
}
