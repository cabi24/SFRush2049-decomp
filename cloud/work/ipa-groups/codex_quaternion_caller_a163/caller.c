/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef signed char s8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef float f32;
struct Camera;
struct CamCtl;
typedef struct CamKey {          /* 0x44 bytes */
    f32 pos[3];                  /* 0x00 */
    f32 dir[3];                  /* 0x0C */
    f32 scale[3];                /* 0x18 */
    f32 rot[4];                  /* 0x24 */
    f32 f34;
    f32 dur;                     /* 0x38 */
    f32 f3C;
    s32 flags;                   /* 0x40 */
} CamKey;
typedef struct CamScene {
    u8 pad00[0x10];
    s32 flags;                   /* 0x10 */
    s16 count;                   /* 0x14 */
    s16 id;                      /* 0x16 */
    struct CamScene *link;       /* 0x18 */
    CamKey *keys;                /* 0x1C */
    struct CamCtl *cur;          /* 0x20 */
} CamScene;
typedef struct CamCtl {
    CamScene *scene;             /* 0x00 */
    f32 t;                       /* 0x04 */
    u8 pad08[4];
    s16 idx;                     /* 0x0C */
    u16 mode;                    /* 0x0E */
    f32 f10;                     /* 0x10 */
    f32 f14;
    f32 look[3];                 /* 0x18 */
    f32 mat[9];                  /* 0x24 */
} CamCtl;
typedef struct Camera {
    u8 pad00[4];
    u8 flags;                    /* 0x04 */
    u8 pad05[9];
    s16 slot;                    /* 0x0E */
    s16 tbl;                     /* 0x10 */
    u8 pad12[2];
    f32 m[3][3];                 /* 0x14 */
    f32 pos[3];                  /* 0x38 */
    u8 pad44[0xC];
    s16 s50;
    u8 pad52[6];
    s16 s58;
    s16 s5A;
    u8 pad5C[4];
    s32 handle;                  /* 0x60 */
    s8 state;                    /* 0x64 */
    s8 s65;
    u8 pad66[6];
    CamCtl *ctl;                 /* 0x6C */
} Camera;
void func_800BFBE8(void *,void *,s32);
void math_utility(void *,void *);
void func_800C0828(void *);
void camera_build_view_matrix(s32 idx, Camera *cam) {
    CamCtl *ctl;
    CamScene *sc;
    s32 i;

    ctl = cam->ctl;
    sc = ctl->scene;
    if ((sc->flags & 1) && idx >= sc->count - 1) {
        ctl->mode = 8;
    } else {
        ctl->mode = 4;
    }
    ctl->idx = idx;
    ctl->t = 0.0f;
    ctl->f10 = 0.0f;
    cam->pos[0] = sc->keys[ctl->idx].pos[0];
    cam->pos[1] = sc->keys[ctl->idx].pos[1];
    cam->pos[2] = sc->keys[ctl->idx].pos[2];
    ctl->look[0] = sc->keys[ctl->idx].pos[0];
    ctl->look[1] = sc->keys[ctl->idx].pos[1];
    ctl->look[2] = sc->keys[ctl->idx].pos[2];
    if (sc->flags & 0xC0) {
        sc->flags |= 0x100;
        sc->cur = ctl;
    }
    if (sc->flags & 0x5000) {
        sc->flags |= 0x400;
    }
    func_800BFBE8(cam->m, sc->keys[ctl->idx].rot, 1);
    math_utility(cam->m, ctl->mat);
    func_800C0828(ctl->mat);
    for (i = 0; i < 3; i++) {
        cam->m[0][i] = sc->keys[ctl->idx].scale[0] * cam->m[0][i];
        cam->m[1][i] = sc->keys[ctl->idx].scale[1] * cam->m[1][i];
        cam->m[2][i] = sc->keys[ctl->idx].scale[2] * cam->m[2][i];
    }
}

