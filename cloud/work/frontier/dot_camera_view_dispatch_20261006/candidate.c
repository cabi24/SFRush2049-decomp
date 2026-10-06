/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete NONMATCH research reconstruction of func_800EB028.
 * Native extent: 0x800EB028..0x800EB690, 1640 bytes.
 * Ancestor: rushtherock game/camera.c::setcamview, commit 845329d7.
 * N64 adaptation uses per-car/per-view records and Y-up coordinates.
 * All called functions retain their real external O32 boundaries.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef f32 Vec3[3];
typedef f32 Mat3[3][3];

typedef struct CarData {
    u8 unknown0[8];
    Vec3 position;
    u8 unknown20[24];
    Mat3 world_basis;
    Mat3 object_basis;
    u8 unknown116[743];
    s8 model_index;
    s8 view_slot;
    u8 view;
    u8 unknown862[90];
} CarData;

typedef struct ModelData {
    u8 unknown0[328];
    f32 peak_body_force[2][3];
    f32 peak_center_force[2][3];
    u8 unknown376[1356];
    s16 collision_state;
    u8 unknown1734[322];
} ModelData;

typedef struct CameraData {
    Mat3 basis;
    Vec3 position;
    u8 unknown48[48];
    Mat3 follow_basis;
    Vec3 follow_position;
    f32 spring;
    u8 unknown148[4];
} CameraData;

typedef struct OSThread OSThread;
extern OSThread D_80034840;
extern ModelData D_8014A250[];
extern CameraData D_80150B70[];
extern Vec3 D_801526A8[];
extern f32 D_80110688, D_8011068C;
extern f32 D_80110690, D_80110694, D_80110698;
extern f32 D_8011069C, D_801106A0;

/* The historical osPfsChecker_full name at 8000BD90 is actually osStopThread. */
extern void osPfsChecker_full(OSThread *);
extern void osStartThread(OSThread *);
extern void math_utility(void *, void *);
extern void func_8009E820(f32 *, f32 *, Mat3 *);
extern void func_800E8CB8(CarData *, f32 *, Mat3 *);
extern f32 func_800EAFDC(ModelData *);
extern void func_800EA3F4(CarData *, f32 *);
extern void func_800EA2DC(CarData *, f32 *, Mat3 *, f32 *);
extern void func_800EA108(CarData *, f32 *, Mat3 *);
extern void func_800E9E2C(CarData *, f32 *, Mat3 *);
extern void func_800E9C70(s16, CarData *, f32 *, Mat3 *);
extern void func_800E95DC(s16, CarData *, f32 *, Mat3 *);
extern void func_800E8F10(s16, CarData *, f32 *, Mat3 *);
extern s32 entity_iterate(f32 *, f32 *);

/* rushtherock game/vecmath.h, substantive macro unchanged. */
#define veccopy(a,r) {r[0]=a[0]; r[1]=a[1]; r[2]=a[2];}
/* rushtherock LIB/fmath.h, substantive macro unchanged. */
#define AddVector(v1,v2,r) (r[0] = v1[0]+v2[0], r[1] = v1[1]+v2[1], r[2] = v1[2]+v2[2])

void func_800EB028(CarData *car)
{
    s32 i, j;
    f32 pos_in[3], pos_out[3];
    ModelData *m = &D_8014A250[car->model_index];
    s32 slot = car->view_slot;
    CameraData *camera = &D_80150B70[slot];

    veccopy(car->position, camera->position);
    camera->position[1] += 0.25f;

    switch (car->view) {
    case 0:
        func_800E8CB8(car, camera->position, &car->object_basis);
        pos_in[0] = 0.0;
        pos_in[1] = func_800EAFDC(m) + D_80110688;
        pos_in[2] = D_8011068C;
        math_utility(car->world_basis, camera->basis);
        func_8009E820(pos_in, pos_out, &camera->basis);
        AddVector(camera->position, pos_out, camera->position);
        break;

    case 1:
        osPfsChecker_full(&D_80034840);
        /* Arcade's two horizontal axes become X and Z in the N64 port. */
        for (j = 0; j < 3; j += 2) {
            if (m->peak_center_force[0][j] > 100000.0f)
                m->peak_center_force[0][j] = 100000.0f;
            if (m->peak_center_force[1][j] < -100000.0f)
                m->peak_center_force[1][j] = -100000.0f;
            if (m->peak_body_force[0][j] > 100000.0f)
                m->peak_body_force[0][j] = 100000.0f;
            if (m->peak_body_force[1][j] < -100000.0f)
                m->peak_body_force[1][j] = -100000.0f;
            pos_in[j] = m->peak_center_force[0][j] * .00002f;
            pos_in[j] += m->peak_center_force[1][j] * .00002f;
            m->peak_center_force[0][j] = 0;
            m->peak_center_force[1][j] = 0;
            pos_in[j] += m->peak_body_force[0][j] * .00002f;
            pos_in[j] += m->peak_body_force[1][j] * .00002f;
            m->peak_body_force[0][j] = 0;
            m->peak_body_force[1][j] = 0;
        }
        osStartThread(&D_80034840);
        for (i = 0; i < 3; i += 2)
            D_801526A8[slot][i] = D_801526A8[slot][i] * D_8011069C
                + pos_in[i] * (1.0f - D_8011069C);
        D_801526A8[slot][1] = D_801526A8[slot][1] * D_801106A0 - camera->spring;
        pos_in[0] = D_80110690 - D_801526A8[slot][0];
        pos_in[1] = D_80110694 - D_801526A8[slot][1];
        pos_in[2] = D_80110698 - D_801526A8[slot][2];
        math_utility(car->object_basis, camera->basis);
        func_8009E820(pos_in, pos_out, &camera->basis);
        if (pos_out[1] < 0)
            camera->position[1] -= pos_out[1] * .7f;
        func_800E8CB8(car, camera->position, &camera->basis);
        AddVector(camera->position, pos_out, camera->position);
        break;

    case 2:
    case 3:
        veccopy(camera->position, pos_in);
        func_800EA3F4(car, camera->position);
        math_utility(camera->follow_basis, camera->basis);
        veccopy(camera->follow_position, camera->position);
        if (m->collision_state < 0)
            entity_iterate(camera->position, pos_in);
        break;

    case 10:
        veccopy(camera->position, pos_in);
        func_800EA2DC(car, camera->position, &car->object_basis, pos_in);
        math_utility(camera->follow_basis, camera->basis);
        veccopy(camera->follow_position, camera->position);
        entity_iterate(camera->position, pos_in);
        break;

    case 8:
        veccopy(camera->position, pos_in);
        func_800EA2DC(car, camera->position, &car->object_basis, pos_in);
        math_utility(camera->follow_basis, camera->basis);
        veccopy(camera->follow_position, camera->position);
        break;

    case 4:
        func_800EA108(car, camera->position, &car->object_basis);
        veccopy(camera->follow_position, camera->position);
        math_utility(camera->follow_basis, camera->basis);
        entity_iterate(camera->position, car->position);
        break;

    case 5:
        func_800E9E2C(car, camera->position, &car->object_basis);
        veccopy(camera->follow_position, camera->position);
        math_utility(camera->follow_basis, camera->basis);
        break;

    case 6:
        func_800E9C70(2, car, camera->position, &car->object_basis);
        veccopy(camera->follow_position, camera->position);
        math_utility(camera->follow_basis, camera->basis);
        break;

    case 7:
        func_800E95DC(2, car, camera->position, &car->object_basis);
        veccopy(camera->follow_position, camera->position);
        math_utility(camera->follow_basis, camera->basis);
        break;

    case 9:
        func_800E8F10(2, car, camera->position, &car->object_basis);
        veccopy(camera->follow_position, camera->position);
        math_utility(camera->follow_basis, camera->basis);
        break;
    }
}
