/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800EB90C (0x800EB90C, 2,000 B): per-frame camera view selection for every viewport = arcade
 * game/camera.c CheckCameraView() (+ the suspension average and the drone-camera placement of setcamview()).
 *  - In gameplay mode 2 with D_8015697C bit 1 and D_8015699C bit 2: bump viewport 0's target car to the next
 *    active car (arcade "bump target drone"; car record D_80152818[] 952 B, model D_8014A250[] 0x808 B).
 *  - Per viewport i < D_80151AD0 (camera D_80150B70[i], its target car obj/model, player record pl):
 *    demo/attract (state_word_a & 8): every D_801106B4[slot] seconds (D_801106A4 = last switch time,
 *    D_801543CC = race clock) and if viGetTimeToDeadline() > 3, pick a random view (arcade
 *    `switch ((long)Random(11.0))`, here (u32), 0..4 + default: fixed-on-track via world_object_destroy
 *    = respawn-point picker, lag 3 s); otherwise D_80110680[player] forces view 5, else the view button
 *    (pl->edges & pl->mask) cycles views 0..3 (func_800CC8C8 notifies the player's object, view 1 clears
 *    cur_acc D_801526A8, view 2/3 re-init View3Cam via func_800E9234). Then func_800EB028 (setcamview's
 *    switch) and the tire-suspension average (D_801525F8 count / D_80152680 avg, gSusAvg).
 *  - func_800CFCA8(), then place the drone cameras (D_8012E700[node].cam) at each active car + 0.25 up.
 * rand/Random are verbatim copies of the locked kept func_8008B2B4/func_8008B2E4 (inlined in the unit, as in
 * func_800B23E0); node_camera is an inlined static (its s16 parameter is retail's a0 narrowing).
 * Shaping devices (disclosed):
 *  - SIX UNUSED LOCALS (new_view3, block, delta, extra_lag, edge, view3_lim -- the names of arcade
 *    CheckCameraView's locals, which the N64 port dropped the uses of): exact frame residual. Retail frame
 *    200; without them 176 (`v/p7.c`, otherwise code-identical apart from the i home slot); 6 slots
 *    (5 gives 192). The 7 used locals declared before i put i's home at 168(sp) as in retail.
 *  - `extern volatile s16 D_801543CA` (car count): retail reloads it every iteration of the bump loop
 *    through a hoisted address (lh t4,0(a0)); non-volatile hoists the load (459 words).
 * Other natural levers: index-form bump loop (`D_80152818[n].slot`, strength-reduced as in retail), both
 * suspension resets on one line and `pl++` in the for header (as1 line tie-breaks), `= 0` (not 0.0f) for
 * D_80152680 (separate zero web from the $f22 0.0f), `Random(1.0f) + 2.5` (double add as in retail).
 * Own .rodata: jump table 0x8012455C (5 entries).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef f32 Vec3[3];
typedef f32 Mat3[3][3];
typedef struct {
    u8 other0[8];
    Vec3 pos;           /* 8 */
    u8 other20[60];
    Mat3 uvs;           /* 80 */
    u8 other116[128];
    s32 node;           /* 244 */
    u8 other248[611];
    s8 car;             /* 859 */
    s8 slot;            /* 860 */
    s8 view;            /* 861 */
    s8 view2;           /* 862 */
    u8 other863[89];
} Object;               /* 952 */
typedef struct {
    u8 other0[1484];
    f32 sus[4];         /* 1484 */
    u8 other1500[16];
    f32 comp[4];        /* 1516 */
    u8 other1532[460];
    s16 active;         /* 1992 */
    u8 other1994[2];
    s8 f1996;           /* 1996 */
    u8 other1997[18];
    s8 f2015;           /* 2015 */
    u8 other2016[40];
} Model;                /* 0x808 */
typedef struct {
    Mat3 uvs;           /* 0 */
    Vec3 pos;           /* 36 */
    u8 other48[48];
    Mat3 out_uvs;       /* 96 */
    Vec3 out_pos;       /* 132 */
    f32 spring;         /* 144 */
    u8 target;          /* 148 */
    u8 other149[3];
} Camera;               /* 152 */
typedef struct { u8 other0[44]; s32 resource; } HObject;
typedef struct { HObject *object; } Handle;
typedef struct {
    u8 index;
    u8 other1[3];
    s32 mask;           /* 4 */
    u8 other8[44];
    s32 edges;          /* 52 */
    u8 other56[16];
    Handle *handle;     /* 72 */
} Player76;
typedef struct { u8 other0[8]; Camera *cam; u8 other12[56]; } Node68;
extern s32 gameplay_mode;
extern s32 D_8015697C, D_8015699C;
extern s32 state_word_a;
extern volatile s16 D_801543CA;
extern s16 D_80151AD0;
extern f32 D_801543CC;
extern Object D_80152818[];
extern Model D_8014A250[];
extern Camera D_80150B70[];
extern Player76 input_rec0[];
extern Vec3 D_801526A8[];
extern f32 D_801106A4[], D_801106B4[];
extern s8 D_80110680[];
extern s8 D_801525F8[];
extern f32 D_80152680[];
extern Node68 D_8012E700[];
extern s32 D_8011735C;
extern f32 viGetTimeToDeadline(void);
extern void func_800CC8C8(Handle *handle, u8 view);
extern void func_800E9234(Object *object);
extern void world_object_destroy(Object *object);
extern void func_800EB028(Object *object);
extern void func_800CFCA8(void);
extern void math_utility(void *src, void *dst);

static int rand(void)
{
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (D_8011735C >> 16) & 0x7fff;
}

static f32 Random(f32 max)
{
    f32 rannum;

    rannum = ((f32)(rand() & 0x07FFF) * max) / 32768.0f;
    return rannum;
}

static Camera *node_camera(s16 node)
{
    return D_8012E700[node].cam;
}

void func_800EB90C(void)
{
    s16 j;
    s32 k;
    s32 view;
    s32 slot;
    Object *obj;
    Model *m;
    Camera *cam;
    s32 i;
    Player76 *pl;
    s32 n;
    s32 new_view3, block, delta, extra_lag; /* unused: see header (frame residual) */
    u32 edge;                               /* unused */
    f32 view3_lim;                          /* unused */

    if (gameplay_mode == 2 && (D_8015697C & 2) && (D_8015699C & 4)) {
        n = D_80150B70[0].target;
        D_80152818[n].slot = -1;
        do {
            n++;
            if (n >= D_801543CA) {
                n = 0;
            }
        } while (D_8014A250[n].active == 0 && D_80152818[n].slot < 0);
        D_80152818[n].slot = 0;
        D_80150B70[0].target = n;
    }
    pl = input_rec0;
    for (i = 0; i < D_80151AD0; i++, pl++) {
        cam = &D_80150B70[i];
        obj = &D_80152818[cam->target];
        m = &D_8014A250[cam->target];
        view = obj->view;
        slot = obj->slot;
        if (state_word_a & 8) {
            if ((D_801543CC - D_801106A4[slot] > D_801106B4[slot] || D_801543CC < D_801106A4[slot]) && view != 4 && viGetTimeToDeadline() > 3.0f) {
                D_801106A4[slot] = D_801543CC;
                D_801106B4[slot] = Random(1.0f) + 2.5;
                switch ((u32)Random(11.0f)) {
                case 0:
                    view = 0;
                    break;
                case 1:
                    view = 1;
                    D_801526A8[slot][0] = 0.0f;
                    D_801526A8[slot][1] = 0.0f;
                    D_801526A8[slot][2] = 0.0f;
                    break;
                case 2:
                    if (view != 3) view = 2;
                    break;
                case 3:
                    if (view != 2) view = 3;
                    break;
                case 4:
                    func_800E9234(obj);
                    view = 10;
                    break;
                default:
                    view = 6;
                    world_object_destroy(obj);
                    D_801106B4[slot] = 3.0f;
                    break;
                }
            }
        } else if (D_80110680[pl->index] != 0 && view != 4) {
            view = 5;
        } else if (view != 4 && (pl->edges & pl->mask)) {
            view++;
            if (view >= 4) view = 0;
            if (pl->handle->object->resource) {
                func_800CC8C8(pl->handle, view);
            }
            if (view == 1) {
                D_801526A8[obj->slot][0] = 0.0f;
                D_801526A8[obj->slot][1] = 0.0f;
                D_801526A8[obj->slot][2] = 0.0f;
            } else if (view == 2) {
                func_800E9234(obj);
                D_80150B70[obj->slot].out_pos[0] = obj->pos[0];
                D_80150B70[obj->slot].out_pos[1] = obj->pos[1] + 1.0f;
                D_80150B70[obj->slot].out_pos[2] = obj->pos[2];
            } else if (view == 3) {
                func_800E9234(obj);
            }
        }
        if (view != obj->view) {
            obj->view2 = view;
            obj->view = view;
        }
        func_800EB028(obj);
        D_801525F8[slot] = 0; D_80152680[slot] = 0;
        for (k = 0; k < 4; k++) {
            if (m->comp[k] < 4.0f) {
                D_801525F8[slot]++;
                D_80152680[slot] += m->sus[k];
            }
        }
        if (D_801525F8[slot] >= 3) {
            D_80152680[slot] /= (f32)D_801525F8[slot];
        } else {
            D_80152680[slot] = 0;
        }
    }
    func_800CFCA8();
    for (j = 0; j < 6; j++) {
        if (D_8014A250[j].active != 0 && D_80152818[j].node >= 0 && D_8014A250[j].f1996 != 2 && D_8014A250[j].f2015 == 0) {
            cam = node_camera(D_80152818[j].node);
            cam->pos[0] = D_80152818[j].pos[0];
            cam->pos[1] = D_80152818[j].pos[1];
            cam->pos[2] = D_80152818[j].pos[2];
            cam->pos[1] += 0.25f;
            math_utility(D_80152818[j].uvs, cam);
        }
    }
}
