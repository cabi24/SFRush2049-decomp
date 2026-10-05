/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef float f32;
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef union Color4 {
    u32 word;
    u8 b[4];
} Color4;

typedef struct Marker {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 view;
    /* 0x06 */ s16 mesh;
    /* 0x08 */ s16 player;
} Marker;

typedef struct Car {                 /* D_80152818[], 0x3B8 */
    /* 0x000 */ u8 pad0[8];
    /* 0x008 */ f32 pos[3];
    /* 0x014 */ u8 pad14[0x344];
    /* 0x358 */ s8 hidden;
    /* 0x359 */ u8 pad359[0x33];
    /* 0x38C */ u32 flags;
    /* 0x390 */ u8 pad390[0x10];
    /* 0x3A0 */ s8 colorIndex;
    /* 0x3A1 */ u8 alpha;
    /* 0x3A2 */ s8 alphaMode;
    /* 0x3A3 */ u8 pad3A3[0x15];
} Car;

typedef struct Model {               /* D_8014A250[], 0x808 */
    /* 0x000 */ u8 pad0[0x6C4];
    /* 0x6C4 */ s16 state;
    /* 0x6C6 */ u8 pad6C6[0x100];
    /* 0x7C6 */ s16 slot;
    /* 0x7C8 */ u8 pad7C8[4];
    /* 0x7CC */ s8 mode;
    /* 0x7CD */ u8 pad7CD[0x3B];
} Model;

typedef struct ViewCam {             /* D_80150B70[], 0x98 */
    /* 0x00 */ f32 proj[9];
    /* 0x24 */ f32 pos[3];
    /* 0x30 */ f32 basis[9];
    /* 0x54 */ u8 pad54[0x44];
} ViewCam;

typedef struct ViewInfo {            /* D_8017A510[], 0x48 */
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ f32 radius;
    /* 0x10 */ f32 scale;
    /* 0x14 */ u8 pad14[0x34];
} ViewInfo;

typedef struct Mesh {                /* D_8015B268[], 0x58 */
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ u16 flags;
    /* 0x04 */ u8 pad4[0x54];
} Mesh;

extern Car D_80152818[];
extern Model D_8014A250[];
extern ViewCam D_80150B70[];
extern ViewInfo D_8017A510[];
extern Mesh D_8015B268[];
extern s8 D_801613A8;
extern s8 D_8017A63C;
extern s8 D_801613C0[][4];
extern s8 D_8012E67C[];
extern s32 D_801174B4;
extern s32 D_8014A110;
extern Color4 D_8011B558[];
extern u8 D_8011B56C[];
extern f32 D_8011AD90[4][3];

void func_8008C074(Mesh *mesh, s32 count, f32 *positions, s32 parameter, u8 *color, s32 flags, s32 indexed);
void func_8008C544(f32 *v, f32 *out, f32 *m);
f32 func_8008C768(f32 y, f32 x);
f32 func_8008B3C8(f32 *v);
void entity_render_setup(Marker *marker, s32 arg1)
{
    Car *car;
    Model *model;
    f32 quad[4][3];
    f32 size;
    s32 view;
    s32 pad;
    Color4 color;
    f32 delta[3];
    f32 angle;
    f32 yaw;
    f32 depth;
    f32 x;
    f32 y;
    f32 slope;
    f32 limit;
    f32 gain;
    s32 side;
    s32 i;
    ViewCam *cam;
    f32 scale;

    car = &D_80152818[marker->player];
    model = &D_8014A250[marker->player];
    color = D_8011B558[8];
    if (!D_801613A8 || !(D_801174B4 & 0x400000) || ((car->hidden || model->state >= 0) && D_8014A110 == 6)) {
        D_8015B268[marker->mesh].flags |= 0x8000;
        return;
    }
    if (D_8014A110 == 6 && (car->flags & 1)) {
        if (car->alphaMode == 1) {
            color.b[3] = car->alpha;
        } else if (car->alphaMode == 2) {
            color.b[3] = car->alpha;
        } else {
            color.b[3] = D_8011B56C[car->colorIndex];
        }
        if (color.b[3] < 32) {
            color.b[3] = 32;
        }
    }
    if (model->mode == 2) {
        if (D_8014A110 == 6) {
            color.b[0] = D_8011B558[D_8012E67C[model->slot]].b[0];
            color.b[1] = D_8011B558[D_8012E67C[model->slot]].b[1];
            color.b[2] = D_8011B558[D_8012E67C[model->slot]].b[2];
        } else {
            color.b[0] = D_8011B558[model->slot].b[0];
            color.b[1] = D_8011B558[model->slot].b[1];
            color.b[2] = D_8011B558[model->slot].b[2];
        }
        if (D_801613C0[model->slot][view] && color.b[3] > 112) {
            color.b[3] = 112;
        }
        if (D_8014A110 == 2 && marker->player > 0) {
            color.b[3] = 128;
        }
    } else {
        color.b[0] = D_8011B558[4].b[0];
        color.b[1] = D_8011B558[4].b[1];
        color.b[2] = D_8011B558[4].b[2];
    }
    cam = &D_80150B70[view = marker->view];
    delta[0] = car->pos[0] - cam->pos[0];
    delta[1] = car->pos[1] - cam->pos[1];
    delta[2] = car->pos[2] - cam->pos[2];
    if (D_8014A110 == 6) {
        func_8008C544(delta, quad[0], cam->proj);
        angle = func_8008C768(-quad[0][0], quad[0][2]);
        depth = 31.415927f / D_8017A510[view].radius;
        yaw = angle;
        if (D_8017A63C >= 2) {
            slope = 16.5f;
            scale = D_8017A510[view].scale;
            limit = scale * 0.75f;
        } else {
            slope = 14.5f;
            x = D_8017A510[view].scale;
            scale = x;
            limit = x * 1.1f;
        }
        x = 0.48f;
        if (angle < -D_8017A510[view].radius * x && -1.5707964f < angle) {
            gain = 0.63661975f;
            x = D_8017A510[view].radius * gain * 19.0f;
            yaw = -angle;
            side = 3;
            goto vertical;
        }
        if (angle < -1.5707964f) {
            yaw = angle + 3.1415927f;
            goto behind;
        }
        if (1.5707964f < angle) {
            yaw = angle - 3.1415927f;
behind:
            gain = 0.63661975f;
            side = 0;
            x = D_8017A510[view].radius * gain * 19.0f * yaw * gain;
            y = scale * gain * slope;
            goto draw;
        }
        if (D_8017A510[view].radius * x < angle && angle < 1.5707964f) {
            gain = 0.63661975f;
            side = 1;
            x = D_8017A510[view].radius * gain * -19.0f;
        } else {
            goto world;
        }
vertical:
        y = scale * gain * slope * (yaw - limit) * (1.0f / (1.5707964f - limit));
draw:
        for (i = 0; i < 4; i++) {
            quad[side][0] = D_8011AD90[i][0] + x;
            quad[side][2] = depth;
            quad[side][1] = D_8011AD90[i][1] - y;
            side = (side + 1) & 3;
        }
        func_8008C074(&D_8015B268[marker->mesh], 4, quad[0], 0, color.b, (1 << view) | 0x3610, 0);
        return;
    }
world:
    size = func_8008B3C8(delta) / 25.0f;
    for (i = 0; i < 4; i++) {
        func_8008C544(D_8011AD90[i], quad[i], cam->basis);
        quad[i][0] = car->pos[0] + quad[i][0] * size;
        quad[i][1] = car->pos[1] + quad[i][1] * size + size + 5.0f;
        quad[i][2] = car->pos[2] + quad[i][2] * size;
    }
    if (D_8014A110 == 6) {
        func_8008C074(&D_8015B268[marker->mesh], 4, quad[0], 0, color.b, (1 << view) | 0x3600, 0);
    } else {
        func_8008C074(&D_8015B268[marker->mesh], 4, quad[0], 0, color.b, (1 << view) | 0x1600, 0);
    }
}
