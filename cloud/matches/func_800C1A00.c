/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800C1A00: velocity of the scripted camera path that drives scene `id`.
 * Walks the cameras registered by camera_process_input (D_8013C300[0..D_8013F1DC-1], scenes
 * with flag 0x20); for the first whose scene id matches, writes the current key's direction
 * vector (keys[ctl->idx].dir) scaled by the controller's rate ctl->f10, negated while the
 * controller runs backwards (mode bit 8).  Writes zero when no camera matches.
 * Types follow the camera structures of src/blob/groups/camera_aspect_ratio.  No arcade ancestor
 * identified.  Plain C, no shaping quirks; also matches at -O2.
 */
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned char u8;
typedef float f32;

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
} CamScene;
typedef struct CamCtl {
    CamScene *scene;             /* 0x00 */
    f32 t;                       /* 0x04 */
    u8 pad08[4];
    s16 idx;                     /* 0x0C */
    u16 mode;                    /* 0x0E */
    f32 f10;                     /* 0x10 */
} CamCtl;
typedef struct Camera {
    u8 pad00[0x6C];
    CamCtl *ctl;                 /* 0x6C */
} Camera;

extern Camera *D_8013C300[];
extern s32 D_8013F1DC;

void func_800C1A00(s32 id, f32 *out) {
    s32 i;
    CamCtl *ctl;
    CamScene *sc;

    out[0] = 0.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;
    for (i = 0; i < D_8013F1DC; i++) {
        ctl = D_8013C300[i]->ctl;
        sc = ctl->scene;
        if (id == sc->id) {
            if (ctl->mode & 8) {
                out[0] = sc->keys[ctl->idx].dir[0] * -ctl->f10;
                out[1] = sc->keys[ctl->idx].dir[1] * -ctl->f10;
                out[2] = sc->keys[ctl->idx].dir[2] * -ctl->f10;
                return;
            }
            out[0] = sc->keys[ctl->idx].dir[0] * ctl->f10;
            out[1] = sc->keys[ctl->idx].dir[1] * ctl->f10;
            out[2] = sc->keys[ctl->idx].dir[2] * ctl->f10;
            return;
        }
    }
}
