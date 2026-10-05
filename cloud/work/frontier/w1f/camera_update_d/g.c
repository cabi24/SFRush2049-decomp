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
f32 func_8008B3F4(f32 value) {
    if (value < 0.0001f) {
        value = 0.0001f;
    }
    return 1.0f / sqrtf(value);
}

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define FTOFRAC8(x) ((s32) MIN(((x) * 128.0f), 127.0f) & 0xFF)

void camera_update_d(LookAt *l, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt,
                     f32 xUp, f32 yUp, f32 zUp) {
    f32 len, xRight, yRight, zRight, xLook, yLook, zLook;


    xLook = xAt - xEye;
    yLook = yAt - yEye;
    zLook = zAt - zEye;
    len = -func_8008B3F4(xLook * xLook + yLook * yLook + zLook * zLook);
    xLook *= len;
    yLook *= len;
    zLook *= len;

    xRight = yUp * zLook - zUp * yLook;
    yRight = zUp * xLook - xUp * zLook;
    zRight = xUp * yLook - yUp * xLook;
    len = func_8008B3F4(xRight * xRight + yRight * yRight + zRight * zRight);
    xRight *= len;
    yRight *= len;
    zRight *= len;

    xUp = yLook * zRight - zLook * yRight;
    yUp = zLook * xRight - xLook * zRight;
    zUp = xLook * yRight - yLook * xRight;
    l->l[0].dir[0] = FTOFRAC8(xRight);
    l->l[0].dir[1] = FTOFRAC8(yRight);
    l->l[0].dir[2] = FTOFRAC8(zRight);
    l->l[1].dir[0] = FTOFRAC8(xUp);
    l->l[1].dir[1] = FTOFRAC8(yUp);
    l->l[1].dir[2] = FTOFRAC8(zUp);

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
