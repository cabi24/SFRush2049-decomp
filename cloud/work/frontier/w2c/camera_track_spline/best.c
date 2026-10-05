/*
 * PROVISIONAL (frontier wave 2, w2c): camera_track_spline proven with a STAND-IN caller. Not spliceable.
 *
 * camera_track_spline(cam): advance a scripted camera along its current key segment. The camera's
 * controller (cam->ctl, 0x6C) points at a scene (key count, flags, 0x44-byte keys). Distance along the
 * key direction is either linear (key flag 0x10000008: f34 * t / dur) or constant acceleration between
 * this key's and the next key's speed (a * t + t^2 * (b - a) / (2 * dur)); t runs backwards when
 * ctl->mode & 8. The offset is snapped to 1/32 units, added to the key position, the per-frame speed is
 * stored in ctl->f10 (distance moved / D_8002EB94), and when the scene has flag 0x20 the move is reported
 * through func_800C0294(scene id, delta) unless (gameplay_mode == 5 and) the camera is more than 450
 * units from player 0.
 *
 * The function is internal in retail: it takes `cam` in a3 and homes no argument, so it only reproduces
 * with a caller in the same -O3 unit. The register comes from the callee's own colouring (idx a0, ctl a1,
 * key a2, cam a3), not from the caller, so any caller gives the same body; the real callers
 * (camera_update, 0x800C0AC4-range, unmatched) are still needed for a real claim.
 *
 * Shaping quirks (all compile-affecting):
 *   - `k = &sc->keys[idx]` before `n`/`keys` are read (computes the key address before the early-out);
 *   - `next = idx + 1` only in the acceleration branch;
 *   - the next key's speed is read into `d` (one variable for b and the result), `dv = d - a` before t;
 *   - `k->dur * 2` with an INT literal keeps the multiply (2.0f becomes x + x);
 *   - both arms of `if (d < a)` are the same statement, as in retail;
 *   - the snap loop is `for (i = 0; &v[i] < &v[3]; i++)` (address compare gives sltu; `i < 3` gives beq, a
 *     pointer variable makes uopt keep &v in a register for the later call) with an int temporary `j`;
 *   - `d` is reused for the squared distance to player 0 (puts t in f2, d in f0);
 *   - `extern volatile f32 D_8002EB94` for the address-form read;
 *   - `f32 u[3]` is unused: it supplies 16 bytes of frame (retail: 128). What retail really declares there
 *     is unknown.
 * Own literal 202500.0f = 0x4845C100 equals the retail word at 0x80123E88.
 */
typedef float f32;
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

float sqrtf(float);
#pragma intrinsic (sqrtf)

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
    u8 pad00[0x38];
    f32 pos[3];                  /* 0x38 */
    u8 pad44[0x28];
    CamCtl *ctl;                 /* 0x6C */
} Camera;

typedef struct GameCar {
    u8 pad0[8];
    f32 pos[3];                  /* 0x08 */
    u8 pad14[0x3A4];
} GameCar; /* 0x3B8 */

extern GameCar player_array[];   /* 0x80152818 */
extern s32 gameplay_mode;        /* 0x8015A110 */
extern volatile f32 D_8002EB94;
void func_800C0294(s32 arg0, f32 *arg1);

void camera_track_spline(Camera *cam);

void camera_track_spline(Camera *cam) {
    f32 v[3];
    f32 w[3];
    f32 u[3];
    CamCtl *ctl;
    CamScene *sc;
    CamKey *k;
    CamKey *keys;
    s32 idx;
    s32 n;
    s32 next;
    s32 near;
    f32 t;
    f32 d;
    f32 a;
    f32 c;
    f32 dv;
    s32 i;
    s32 j;

    ctl = cam->ctl;
    idx = ctl->idx;
    sc = ctl->scene;
    k = &sc->keys[idx];
    n = sc->count;
    keys = sc->keys;
    if (idx < n - 1 || (sc->flags & 2) != 0 || !(sc->flags & 0x80)) {
        if (k->flags & 0x10000008) {
            if (ctl->mode & 8) {
                t = k->dur - ctl->t;
            } else {
                t = ctl->t;
            }
            d = k->f34 * (t / k->dur);
        } else {
            next = idx + 1;
            if (next >= n && (sc->flags & 2)) {
                next = 0;
            }
            a = k->f3C;
            d = keys[next].f3C;
            dv = d - a;
            if (ctl->mode & 8) {
                t = k->dur - ctl->t;
            } else {
                t = ctl->t;
            }
            c = (t * t * dv) / (k->dur * 2);
            if (d < a) {
                d = a * t + c;
            } else {
                d = a * t + c;
            }
        }
        v[0] = k->dir[0];
        v[1] = k->dir[1];
        v[2] = k->dir[2];
        ctl->look[0] = cam->pos[0];
        ctl->look[1] = cam->pos[1];
        ctl->look[2] = cam->pos[2];
        v[0] = v[0] * d;
        v[1] = v[1] * d;
        v[2] = v[2] * d;
        for (i = 0; &v[i] < &v[3]; i++) {
            j = v[i] * 32.0f;
            v[i] = j * 0.03125f;
        }
        cam->pos[0] = k->pos[0] + v[0];
        cam->pos[1] = k->pos[1] + v[1];
        cam->pos[2] = k->pos[2] + v[2];
        v[0] = cam->pos[0] - ctl->look[0];
        v[1] = cam->pos[1] - ctl->look[1];
        v[2] = cam->pos[2] - ctl->look[2];
        ctl->f10 = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]) / D_8002EB94;
        if (sc->flags & 0x20) {
            near = 1;
            if (gameplay_mode == 5) {
                w[0] = cam->pos[0] - player_array[0].pos[0];
                w[1] = cam->pos[1] - player_array[0].pos[1];
                w[2] = cam->pos[2] - player_array[0].pos[2];
                d = w[0] * w[0] + w[1] * w[1] + w[2] * w[2];
                if (202500.0f < d) {
                    near = 0;
                }
            }
            if (near != 0) {
                func_800C0294(sc->id, v);
            }
        }
    }
}

/* STAND-IN caller (not a real function): keeps camera_track_spline internal with two call sites. */
extern s32 D_STANDIN_w2c;
void __standin_camera_track_spline_caller(s32 a, s32 b, s32 c, Camera *cam)
{
    if (a) {
        camera_track_spline(cam);
    } else {
        camera_track_spline(cam);
        D_STANDIN_w2c = b + c;
    }
}
