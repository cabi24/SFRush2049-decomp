/* AnimateArrow: source-backed original local order and genuine HidePoly context. */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;

typedef struct Vec3 { f32 x, y, z; } Vec3;
typedef union Color4 {struct {u8 r,g,b,a;} c;u32 rgba;} Color4;
typedef struct Effect {
    struct Effect *next;
    s16 view;       /* 4 */
    s16 mesh;       /* 6 */
    s16 player;     /* 8 */
    u8 pad10[2];
    void *data;
    f32 time;
    void (*callback)(struct Effect *, s16);
} Effect;
typedef struct CAR_DATA {
    u8 pad0[8];
    f32 pos[3];             /* 8 */
    u8 pad14[856 - 20];
    s8 hidden;              /* 856 */
    u8 pad359[908 - 857];
    u32 flags;              /* 908 */
    u8 pad390[928 - 912];
    s8 color_index;         /* 928 */
    u8 alpha;               /* 929 */
    s8 alpha_mode;          /* 930 */
    u8 pad3A3[952 - 931];
} CAR_DATA;
typedef struct MODELDAT {
    u8 pad0[1732];
    s16 state;              /* 1732 */
    u8 pad6C6[1990 - 1734];
    s16 player;             /* 1990 */
    u8 pad7C8[1996 - 1992];
    s8 mode;                /* 1996 */
    u8 pad7CD[2056 - 1997];
} MODELDAT;
typedef struct Camera {
    f32 mat[3][3];             /* 0 */
    Vec3 pos;               /* 36 */
    f32 basis[3][3];           /* 48 */
    u8 pad54[152 - 84];
} Camera;
typedef struct View {
    u8 pad0[12];
    f32 radius;             /* 12 */
    f32 scale;              /* 16 */
    u8 pad14[72 - 20];
} View;
typedef struct Vertex20 {f32 x,y,z;s16 s,t;u32 color;} Vertex20;
typedef struct Mesh {s16 count;u16 flags,parameter,unknown;Vertex20 vertices[4];} Mesh;

extern CAR_DATA D_80152818[];
extern MODELDAT D_8014A250[];
extern Camera D_80150B70[];
extern View D_8017A510[];
extern Mesh D_8015B268[];
extern s8 D_801613A8, D_8017A63C, D_801613C0[4][4], D_8012E67C[];
extern s32 state_word_a, D_8014A110;
extern Color4 D_8011B558[5];
extern Color4 D_8011B578;
extern u8 D_8011B56C[];
f32 D_8011AD90[4][3] = { { 1.0f, 1.0f, 0.0f }, { -1.0f, 1.0f, 0.0f }, { -1.0f, -1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f } };
extern void func_8008C074(Mesh *, s32, f32 (*)[3], u16, Color4 *, u16, s32);
extern void func_8008C544(f32 *, f32 *, f32 (*)[3]);
extern f32 func_8008C768(f32, f32);
extern f32 func_8008B3C8(f32 *);

static void ZOID_HidePoly(Mesh *poly){poly->flags |= 0x8000;}
void entity_render_setup(Effect *effect, s16 op) {
    CAR_DATA *record;
    MODELDAT *vehicle;
    f32 quad[4][3];
    f32 angle;
    s32 type;
    s32 i;
    Color4 color;
    f32 delta[3];
    View *view;
    f32 x, y, depth, len, slope, threshold;
    u32 side;

    record = &D_80152818[effect->player];
    vehicle = &D_8014A250[effect->player];
    type = effect->view;
    color = D_8011B578;
    if (!D_801613A8 || !(state_word_a & 0x400000) || (!(!record->hidden && vehicle->state < 0) && D_8014A110 == 6)) {
        ZOID_HidePoly(&D_8015B268[effect->mesh]);
        return;
    }
    if (D_8014A110 == 6 && (record->flags & 1)) {
        if (record->alpha_mode == 1) {
            color.c.a = record->alpha;
        } else if (record->alpha_mode == 2) {
            color.c.a = record->alpha;
        } else {
            color.c.a = D_8011B56C[record->color_index];
        }
        if (color.c.a < 32) {
            color.c.a = 32;
        }
    }
    if (vehicle->mode == 2) {
        if (D_8014A110 == 6) {
            color.c.r = D_8011B558[D_8012E67C[vehicle->player]].c.r;
            color.c.g = D_8011B558[D_8012E67C[vehicle->player]].c.g;
            color.c.b = D_8011B558[D_8012E67C[vehicle->player]].c.b;
        } else {
            color.c.r = D_8011B558[vehicle->player].c.r;
            color.c.g = D_8011B558[vehicle->player].c.g;
            color.c.b = D_8011B558[vehicle->player].c.b;
        }
        if (D_801613C0[vehicle->player][type] && color.c.a > 112) {
            color.c.a = 112;
        }
        if (D_8014A110 == 2 && effect->player > 0) {
            color.c.a = 128;
        }
    } else {
        color.c.r = D_8011B558[4].c.r;
        color.c.g = D_8011B558[4].c.g;
        color.c.b = D_8011B558[4].c.b;
    }
    delta[0] = record->pos[0] - D_80150B70[type].pos.x;
    delta[1] = record->pos[1] - D_80150B70[type].pos.y;
    delta[2] = record->pos[2] - D_80150B70[type].pos.z;
    if (D_8014A110 == 6) {
        view = &D_8017A510[type];
        func_8008C544(&delta[0], &quad[0][0], D_80150B70[type].mat);
        angle = func_8008C768(-quad[0][0], quad[0][2]);
        len = 19.0f;
        depth = 31.415927f / view->radius;
        if (D_8017A63C >= 2) {
            slope = 16.5f;
            threshold = view->scale * 0.75f;
        } else {
            slope = 14.5f;
            threshold = view->scale * 1.1f;
        }
        if (angle < -view->radius * 0.48f && angle > -1.5707964f) {
            x = view->radius * 0.63661975f * len;
            angle = -angle;
            side = 3;
            goto tail;
        }
        if (angle < -1.5707964f) {
            angle = angle + 3.1415927f;
            goto behind;
        }
        if (1.5707964f < angle) {
            angle = angle - 3.1415927f;
behind:
            x = view->radius * 0.63661975f * len * angle * 0.63661975f;
            y = view->scale * 0.63661975f * slope;
            side = 0;
            goto draw;
        }
        if (view->radius * 0.48f < angle && angle < 1.5707964f) {
            x = view->radius * 0.63661975f * -len;
            side = 1;
tail:
            y = view->scale * 0.63661975f * slope * (angle - threshold) * (1.0f / (1.5707964f - threshold));
draw:
            for (i = 0; i < 4; i++) {
                quad[side][0] = D_8011AD90[i][0] + x;
                quad[side][1] = D_8011AD90[i][1] - y;
                quad[side][2] = depth;
                side = (side + 1) & 3;
        }
            func_8008C074(&D_8015B268[effect->mesh], 4, quad, 0, &color, (1 << type) | 0x3610, 0);
            return;
        }
    }

    angle = func_8008B3C8(&delta[0]) / 25.0f;
    for (i = 0; i < 4; i++) {
        func_8008C544(&D_8011AD90[i][0], &quad[i][0], D_80150B70[type].basis);
        quad[i][0] = quad[i][0] * angle + record->pos[0];
        quad[i][1] = quad[i][1] * angle + record->pos[1] + angle + 5.0f;
        quad[i][2] = quad[i][2] * angle + record->pos[2];
    }
    if (D_8014A110 == 6) {
        func_8008C074(&D_8015B268[effect->mesh], 4, quad, 0, &color, (1 << type) | 0x3600, 0);
    } else {
        func_8008C074(&D_8015B268[effect->mesh], 4, quad, 0, &color, (1 << type) | 0x1600, 0);
    }
}
