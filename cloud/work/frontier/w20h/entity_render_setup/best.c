/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * entity_render_setup -- NEAR-MISS (w20h, from w17d), not a match.
 * Change vs w17d: D_8011AD90 (the 4-corner quad template, retail bytes {1,1,0},{-1,1,0},{-1,-1,0},{1,-1,0})
 * is DEFINED here (initialised .data) instead of extern. In retail's whole-program unit every data symbol is
 * defined, and as1 shares one `lui at` between loads of a defined symbol only within one 16-byte-aligned
 * chunk of its section (it cannot know the hi part across a 16-byte boundary). Extern symbols never share.
 * Retail's draw loop (lui {x0,y0,x1} lui {y1,x2,y2} lui {x3,y3}) needs D_8011AD90 at a .data offset = 0 mod 16;
 * in the current unit it lands at .data+0x48 (72 = 8 mod 16), so two extra `lui at` remain (514 words).
 * See w20h/RESULTS.md (asm.sh oracle: data at a 16-aligned offset -> draw loop exact, 513 words).
 * Shaping devices (disclosed, from w17d):
 *   - `op;` compiled-out read of the S16 parameter: produces retail's bare `sw a1,188(sp)` entry home.
 *   - `view = (View *)(u32)&D_8017A510[type];` laundered address (drops `.noalias $3,$sp`).
 *   - `angle` is reused as the world-path scale (one variable, home 124, retail $f14 in both paths).
 *   - `len = 19.0f` constant-propagated variable (retail's neg.s of 19.0 in the C arm).
 * All named locals are used; slots 176 (vehicle), 116 (i), 96..80 (view, depth, gain, x, len), 68, 64 (y, side)
 * are never accessed in retail either (register-only or copy-propagated).
 */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;

typedef struct Vec3 { f32 x, y, z; } Vec3;
typedef struct Color4 { u8 r, g, b, a; } Color4;
typedef struct Effect {
    u8 pad0[4];
    s16 view;       /* 4 */
    s16 mesh;       /* 6 */
    s16 player;     /* 8 */
} Effect;
typedef struct CAR_DATA {
    u8 pad0[8];
    Vec3 pos;               /* 8 */
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
    f32 mat[9];             /* 0 */
    Vec3 pos;               /* 36 */
    f32 basis[9];           /* 48 */
    u8 pad54[152 - 84];
} Camera;
typedef struct View {
    u8 pad0[12];
    f32 radius;             /* 12 */
    f32 scale;              /* 16 */
    u8 pad14[72 - 20];
} View;
typedef struct Mesh {
    u16 pad0;
    u16 flags;              /* 2 */
    u8 pad4[88 - 4];
} Mesh;

extern CAR_DATA D_80152818[];
extern MODELDAT D_8014A250[];
extern Camera D_80150B70[];
extern View D_8017A510[];
extern Mesh D_8015B268[];
extern s8 D_801613A8, D_8017A63C, D_801613C0[], D_8012E67C[];
extern s32 state_word_a, D_8014A110;
extern Color4 D_8011B558[];
extern Color4 D_8011B578;
extern u8 D_8011B56C[];
Vec3 D_8011AD90[4] = { { 1.0f, 1.0f, 0.0f }, { -1.0f, 1.0f, 0.0f }, { -1.0f, -1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f } };
extern void func_8008C074(Mesh *, s32, Vec3 *, s32, u8 *, s32, s32);
extern void func_8008C544(f32 *, f32 *, f32 *);
extern f32 func_8008C768(f32, f32);
extern f32 func_8008B3C8(f32 *);

void entity_render_setup(Effect *effect, s16 op) {
    CAR_DATA *record;
    MODELDAT *vehicle;
    Vec3 quad[4];
    f32 angle;
    s32 type;
    s32 i;
    Color4 color;
    Vec3 delta;
    View *view;
    f32 depth, gain, x, len, slope, threshold, y;
    u32 side;

    op;
    record = &D_80152818[effect->player];
    vehicle = &D_8014A250[effect->player];
    type = effect->view;
    view = (View *)(u32)&D_8017A510[type];
    color = D_8011B578;
    if (!D_801613A8 || !(state_word_a & 0x400000) || (!(!record->hidden && vehicle->state < 0) && D_8014A110 == 6)) {
        D_8015B268[effect->mesh].flags |= 0x8000;
        return;
    }
    if (D_8014A110 == 6 && (record->flags & 1)) {
        if (record->alpha_mode == 1) {
            color.a = record->alpha;
        } else if (record->alpha_mode == 2) {
            color.a = record->alpha;
        } else {
            color.a = D_8011B56C[record->color_index];
        }
        if (color.a < 32) {
            color.a = 32;
        }
    }
    if (vehicle->mode == 2) {
        if (D_8014A110 == 6) {
            color.r = D_8011B558[D_8012E67C[vehicle->player]].r;
            color.g = D_8011B558[D_8012E67C[vehicle->player]].g;
            color.b = D_8011B558[D_8012E67C[vehicle->player]].b;
        } else {
            color.r = D_8011B558[vehicle->player].r;
            color.g = D_8011B558[vehicle->player].g;
            color.b = D_8011B558[vehicle->player].b;
        }
        if (D_801613C0[vehicle->player * 4 + type] && color.a > 112) {
            color.a = 112;
        }
        if (D_8014A110 == 2 && effect->player > 0) {
            color.a = 128;
        }
    } else {
        color.r = D_8011B558[4].r;
        color.g = D_8011B558[4].g;
        color.b = D_8011B558[4].b;
    }
    delta.x = record->pos.x - D_80150B70[type].pos.x;
    delta.y = record->pos.y - D_80150B70[type].pos.y;
    delta.z = record->pos.z - D_80150B70[type].pos.z;
    if (D_8014A110 == 6) {
        func_8008C544(&delta.x, &quad[0].x, D_80150B70[type].mat);
        angle = func_8008C768(-quad[0].x, quad[0].z);
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
            gain = 0.63661975f;
            x = view->radius * gain * len;
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
            gain = 0.63661975f;
            x = view->radius * gain * len * angle * gain;
            y = view->scale * gain * slope;
            side = 0;
            goto draw;
        }
        if (view->radius * 0.48f < angle && angle < 1.5707964f) {
            gain = 0.63661975f;
            x = view->radius * gain * -len;
            side = 1;
tail:
            y = view->scale * gain * slope * (angle - threshold) * (1.0f / (1.5707964f - threshold));
draw:
            for (i = 0; i < 4; i++) {
                quad[side].x = D_8011AD90[i].x + x;
                quad[side].y = D_8011AD90[i].y - y;
                quad[side].z = depth;
                side = (side + 1) & 3;
        }
            func_8008C074(&D_8015B268[effect->mesh], 4, quad, 0, &color.r, (1 << type) | 0x3610, 0);
            return;
        }
    }

    angle = func_8008B3C8(&delta.x) / 25.0f;
    for (i = 0; i < 4; i++) {
        func_8008C544(&D_8011AD90[i].x, &quad[i].x, D_80150B70[type].basis);
        quad[i].x = record->pos.x + quad[i].x * angle;
        quad[i].y = record->pos.y + quad[i].y * angle + angle + 5.0f;
        quad[i].z = record->pos.z + quad[i].z * angle;
    }
    if (D_8014A110 == 6) {
        func_8008C074(&D_8015B268[effect->mesh], 4, quad, 0, &color.r, (1 << type) | 0x3600, 0);
    } else {
        func_8008C074(&D_8015B268[effect->mesh], 4, quad, 0, &color.r, (1 << type) | 0x1600, 0);
    }
}
