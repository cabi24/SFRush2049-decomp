/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800EB028 (0x800EB028, 1,640 B): per-object camera view update = arcade game/camera.c setcamview()'s
 * `switch (view)` (N64 view byte 861, 0..10; slot camera D_80150B70[slot] (152 B), model D_8014A250[car]).
 * Camera position starts at the object position + 0.25 up. 0 worm's eye (update_car_object, view1_suscomp
 * offset func_800EAFDC + 1.2, 4.0 forward, ftransvec = func_8009E820), 1 driver's eye (collision-force clamp
 * and smoothing between osStopThread/osStartThread on D_80034840, x/z only: `j != 4; j += 2`; cur_acc =
 * D_801526A8[slot]; driver offset D_80110690..98; acc elasticity D_8011069C / D_801106A0), 2/3 View3Cam
 * (func_800EA3F4), 8/10 UpdateCarObj (func_800EA2DC), 4 DeathCam (func_800EA108), 5 circle (func_800E9E2C),
 * 6 fixed (func_800E9C70), 7 steady_move_cam (func_800E95DC), 9 maxpath_cam (func_800E8F10); each copies the
 * camera's out matrix/pos back (math_utility = fmatcopy) and some re-ground the camera (entity_iterate).
 * STATE: draft. unit 163/410 words (rows: words 175 ops 21 norm 21), frame 96/96.
 * Residual class: register/schedule. Retail promotes pos_out[1] into $f0 in cases 0/1 (ours reloads it into
 * a ring temp) and the FP temp ring in case 1 runs one pair ahead (f18 where ours has f4) from the loop on;
 * case 2/3/8/10 copy cam->pos into pos_in with all three loads first (ours interleaves); case-1 slot spill 56
 * vs 52. Levers tried without effect: `pos_out[i] + cam->pos[i]` order (canonicalised), a `for` loop for the
 * adds (not unrolled, worse), veccopy helper (worse).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef f32 Vec3[3];
typedef f32 Mat3[3][3];
typedef struct { s32 opaque[0x1B0 / 4]; } OSThread;
typedef struct {
    u8 other0[8];
    Vec3 pos;           /* 8 */
    u8 other20[24];
    Mat3 uvs44;         /* 44 */
    Mat3 uvs;           /* 80 */
    u8 other116[743];
    s8 car;             /* 859 */
    s8 slot;            /* 860 */
    u8 view;            /* 861 */
} Object;
typedef struct {
    u8 other0[328];
    f32 body_force[2][3];   /* 328 */
    f32 center_force[2][3]; /* 352 */
    u8 other376[1356];
    s16 f1732;              /* 1732 */
    u8 other1734[322];
} Model;                    /* 0x808 */
typedef struct {
    Mat3 uvs;           /* 0 */
    Vec3 pos;           /* 36 */
    u8 other48[48];
    Mat3 out_uvs;       /* 96 */
    Vec3 out_pos;       /* 132 */
    f32 spring;         /* 144 */
    u8 other148[4];
} Camera;               /* 152 */
extern Model D_8014A250[];
extern Camera D_80150B70[];
extern Vec3 D_801526A8[];
extern OSThread D_80034840;
extern f32 D_80110688, D_8011068C, D_80110690, D_80110694, D_80110698, D_8011069C, D_801106A0;
extern void osStopThread(OSThread *);
extern void osStartThread(OSThread *);
extern void math_utility(void *src, void *dst);
extern void func_8009E820(f32 *in, f32 *out, void *uvs);
extern f32 func_800EAFDC(Model *m);
extern void func_800E8CB8(Object *object, f32 *pos, void *uvs);
extern void func_800EA3F4(Object *object, f32 *pos);
extern void func_800EA2DC(Object *object, f32 *pos, Mat3 *uvs, f32 *old);
extern void func_800EA108(Object *object, f32 *pos, Mat3 *uvs);
extern void func_800E9E2C(Object *object, f32 *pos, Mat3 *uvs);
extern void func_800E9C70(s16 mode, Object *object, f32 *pos, Mat3 *uvs);
extern void func_800E95DC(s16 mode, Object *object, f32 *pos, Mat3 *uvs);
extern void func_800E8F10(s16 mode, Object *object, f32 *pos, Mat3 *uvs);
extern s32 entity_iterate(void *arg0, void *arg1);

void func_800EB028(Object *object)
{
    s32 i;
    s32 j;
    Vec3 pos_in;
    Vec3 pos_out;
    Model *m;
    Camera *cam;
    s32 slot;

    m = &D_8014A250[object->car];
    slot = object->slot;
    cam = &D_80150B70[slot];
    cam->pos[0] = object->pos[0];
    cam->pos[1] = object->pos[1];
    cam->pos[2] = object->pos[2];
    cam->pos[1] += 0.25f;
    switch (object->view) {
    case 0:
        func_800E8CB8(object, cam->pos, object->uvs);
        pos_in[0] = 0.0f;
        pos_in[1] = func_800EAFDC(m) + D_80110688;
        pos_in[2] = D_8011068C;
        math_utility(object->uvs44, cam);
        func_8009E820(pos_in, pos_out, cam);
        cam->pos[0] += pos_out[0];
        cam->pos[1] += pos_out[1];
        cam->pos[2] += pos_out[2];
        break;
    case 1:
        osStopThread(&D_80034840);
        for (j = 0; j != 4; j += 2) {
            if (m->center_force[0][j] > 100000.0f) m->center_force[0][j] = 100000.0f;
            if (m->center_force[1][j] < -100000.0f) m->center_force[1][j] = -100000.0f;
            if (m->body_force[0][j] > 100000.0f) m->body_force[0][j] = 100000.0f;
            if (m->body_force[1][j] < -100000.0f) m->body_force[1][j] = -100000.0f;
            pos_in[j] = m->center_force[0][j] * 0.00002f;
            pos_in[j] += m->center_force[1][j] * 0.00002f;
            m->center_force[0][j] = 0.0f;
            m->center_force[1][j] = 0.0f;
            pos_in[j] += m->body_force[0][j] * 0.00002f;
            pos_in[j] += m->body_force[1][j] * 0.00002f;
            m->body_force[0][j] = 0.0f;
            m->body_force[1][j] = 0.0f;
        }
        osStartThread(&D_80034840);
        for (i = 0; i != 4; i += 2) {
            D_801526A8[slot][i] = D_801526A8[slot][i] * D_8011069C + pos_in[i] * (1.0f - D_8011069C);
        }
        D_801526A8[slot][1] = D_801526A8[slot][1] * D_801106A0 - cam->spring;
        pos_in[0] = D_80110690 - D_801526A8[slot][0];
        pos_in[1] = D_80110694 - D_801526A8[slot][1];
        pos_in[2] = D_80110698 - D_801526A8[slot][2];
        math_utility(object->uvs, cam);
        func_8009E820(pos_in, pos_out, cam);
        if (pos_out[1] < 0.0f) {
            cam->pos[1] -= pos_out[1] * 0.7f;
        }
        func_800E8CB8(object, cam->pos, cam);
        cam->pos[0] += pos_out[0];
        cam->pos[1] += pos_out[1];
        cam->pos[2] += pos_out[2];
        break;
    case 2:
    case 3:
        pos_in[0] = cam->pos[0];
        pos_in[1] = cam->pos[1];
        pos_in[2] = cam->pos[2];
        func_800EA3F4(object, cam->pos);
        math_utility(cam->out_uvs, cam);
        cam->pos[0] = cam->out_pos[0];
        cam->pos[1] = cam->out_pos[1];
        cam->pos[2] = cam->out_pos[2];
        if (m->f1732 < 0) {
            entity_iterate(cam->pos, pos_in);
        }
        break;
    case 10:
        pos_in[0] = cam->pos[0];
        pos_in[1] = cam->pos[1];
        pos_in[2] = cam->pos[2];
        func_800EA2DC(object, cam->pos, &object->uvs, pos_in);
        math_utility(cam->out_uvs, cam);
        cam->pos[0] = cam->out_pos[0];
        cam->pos[1] = cam->out_pos[1];
        cam->pos[2] = cam->out_pos[2];
        entity_iterate(cam->pos, pos_in);
        break;
    case 8:
        pos_in[0] = cam->pos[0];
        pos_in[1] = cam->pos[1];
        pos_in[2] = cam->pos[2];
        func_800EA2DC(object, cam->pos, &object->uvs, pos_in);
        math_utility(cam->out_uvs, cam);
        cam->pos[0] = cam->out_pos[0];
        cam->pos[1] = cam->out_pos[1];
        cam->pos[2] = cam->out_pos[2];
        break;
    case 4:
        func_800EA108(object, cam->pos, &object->uvs);
        cam->pos[0] = cam->out_pos[0];
        cam->pos[1] = cam->out_pos[1];
        cam->pos[2] = cam->out_pos[2];
        math_utility(cam->out_uvs, cam);
        entity_iterate(cam->pos, object->pos);
        break;
    case 5:
        func_800E9E2C(object, cam->pos, &object->uvs);
        cam->pos[0] = cam->out_pos[0];
        cam->pos[1] = cam->out_pos[1];
        cam->pos[2] = cam->out_pos[2];
        math_utility(cam->out_uvs, cam);
        break;
    case 6:
        func_800E9C70(2, object, cam->pos, &object->uvs);
        cam->pos[0] = cam->out_pos[0];
        cam->pos[1] = cam->out_pos[1];
        cam->pos[2] = cam->out_pos[2];
        math_utility(cam->out_uvs, cam);
        break;
    case 7:
        func_800E95DC(2, object, cam->pos, &object->uvs);
        cam->pos[0] = cam->out_pos[0];
        cam->pos[1] = cam->out_pos[1];
        cam->pos[2] = cam->out_pos[2];
        math_utility(cam->out_uvs, cam);
        break;
    case 9:
        func_800E8F10(2, object, cam->pos, &object->uvs);
        cam->pos[0] = cam->out_pos[0];
        cam->pos[1] = cam->out_pos[1];
        cam->pos[2] = cam->out_pos[2];
        math_utility(cam->out_uvs, cam);
        break;
    }
}
