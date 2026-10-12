/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * menu_controller_remap (0x800CF69C, 1548 B) -- the N64 port of arcade detect_drone_abort()
 * (reference/repos/rushtherock/game/resurrect.c). The name is a historical label.
 * - first 5 s of a race: clear model[i].stuck_time and game_car[i].fwd_progress_t / stray angle;
 * - per active car we control: falling below y = -190 aborts at once (func_800C54F0); otherwise the
 *   arcade "stuck" timer (stuck_time vs model time, wait 6 s x wait_time) triggers the resurrection
 *   (func_800C4F68(2, m, 1) = set_resurrection_data, we_died = 2);
 * - per player viewport (the arcade's single this_node): wrong-way / help-arrow angle, walking the
 *   max path 5 points ahead (func_800B9F60) and fatan2 (func_8008C768) of the body-frame offset.
 * MODELDAT = D_8014A250 (stride 0x808), CAR_DATA = game_car D_80152818 (stride 0x3B8).
 *
 * Disclosed: index1, index2, dist1, dist2, pos[3] and angle are the arcade function's declaration
 * list and are unused here; they are exactly the frame residual: retail frame 168, 144 without them
 * (12 words of sp offsets differ); 24 B for six declarations (2 x s16, 3 x f32, 1 x f32[3], with the
 * frame's 8-byte rounding). Measured: without index1/index2 sp offsets are 4 B off (20 words); without
 * pos[3] as well the frame is 152. The player loop counts with `node` and the 5-step walk with `i` (arcade reuses i the same
 * way); D_8014A118[node].node is written twice (model first), not through a local (temp ring).
 * Own .rodata (0.1f at 0x80124124) verified by score.py. Unit: EQUAL, 0 locked bodies differ.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    u8 pad0[0x3F0];
    f32 magvel;             /* 0x3F0 */
    u8 pad3F4[0x6C4 - 0x3F4];
    s16 unk6C4;             /* 0x6C4 */
    u8 pad6C6[0x6CC - 0x6C6];
    s8 unk6CC;              /* 0x6CC */
    u8 pad6CD[0x714 - 0x6CD];
    f32 time;               /* 0x714 */
    u8 pad718[0x7C6 - 0x718];
    s16 slot;               /* 0x7C6 */
    s16 unk7C8;
    s16 we_control;         /* 0x7CA */
    u8 pad7CC[0x7D8 - 0x7CC];
    f32 stuck_time;         /* 0x7D8 */
    u8 pad7DC[0x7E0 - 0x7DC];
    s16 please_abort_me;    /* 0x7E0 */
    u8 pad7E2[0x7EC - 0x7E2];
    f32 drone_scale;        /* 0x7EC */
    u8 pad7F0[0x808 - 0x7F0];
} MODELDAT;

typedef struct {
    u8 pad0[8];
    f32 dr_pos[3];          /* 0x08 */
    u8 pad14[0x2C - 0x14];
    f32 dr_uvs[3][3];       /* 0x2C */
    u8 pad50[0xEF - 0x50];
    s8 place_locked;        /* 0xEF */
    u8 padF0[0xFA - 0xF0];
    s16 mpi;                /* 0xFA */
    s16 seg;                /* 0xFC */
    s16 padFE;
    f32 distance;           /* 0x100 */
    f32 last_distance;      /* 0x104 */
    u8 pad108[0x340 - 0x108];
    f32 fwd_progress_t;     /* 0x340 */
    u8 pad344[0x358 - 0x344];
    s8 we_died;             /* 0x358 */
    s8 unk359;              /* 0x359 */
    u8 pad35A[0x364 - 0x35A];
    f32 stray_angle;        /* 0x364 */
    u8 pad368[0x3B8 - 0x368];
} CAR_DATA;

typedef struct {
    u8 node;
    u8 pad1[7];
    s32 unk8;
    u8 padC[0x2C - 0xC];
    s32 unk2C;
    u8 pad30[0x4C - 0x30];
} Player;

typedef struct {
    u8 pad0[0xC];
    s16 (*pts)[3];
} PathBranch;

typedef struct {
    u8 pad0[4];
    s16 (*pts)[3];
    u8 pad8[4];
    PathBranch *branch;
} PathData;

typedef struct {
    u8 pad0[7];
    u8 unk7;
} NodeInfo;

extern f32 D_801543CC;
extern s8 D_80152744;
extern MODELDAT D_8014A250[];
extern CAR_DATA D_80152818[];
extern NodeInfo D_80153E88[];
extern s32 D_8014A110;
extern u32 D_801174B4;
extern s8 D_801403D0;
extern s8 D_80152718;
extern s8 D_8013FECB;
extern s16 D_8014A108;
extern Player D_8014A118[];
extern PathData D_801407F0;

void func_800C54F0(s16 node, s32 arg1);
s32 func_800CF604(s16 node);
void func_800C4F68(s32 what, MODELDAT *m, s32 arg2);
void func_800B9F60(s32 idx, s32 seg, s32 *nidx, s32 *nseg);
void func_800A61B0(f32 *in, f32 *out, f32 (*uvs)[3]);
f32 func_8008C768(f32 x, f32 z);

void menu_controller_remap(void) {
    s16 i;
    s16 node;
    s16 abort_me;
    s16 index1;
    s16 index2;
    f32 dist1;
    f32 dist2;
    f32 pos[3];
    f32 pos2[3];
    f32 delta[3];
    f32 angle;
    s32 wait_time;
    MODELDAT *m;
    CAR_DATA *gc;
    s32 idx;
    s32 seg;

    if (D_801543CC < 5.0f) {
        for (i = 0; i < 6; i++) {
            D_8014A250[i].stuck_time = 0.0f;
            D_80152818[i].fwd_progress_t = 0.0f;
            D_80152818[i].stray_angle = 0.0f;
        }
        return;
    }

    for (i = 0; i < D_80152744; i++) {
        node = D_8014A250[i].slot;
        m = &D_8014A250[node];
        gc = &D_80152818[node];
        if (m->we_control || D_80153E88[node].unk7 == 6) {
            abort_me = m->please_abort_me;
            if (gc->dr_pos[1] < -190.0f && gc->unk359 == 0 && gc->we_died == 0 && m->unk6C4 == -1) {
                m->unk6CC = 1;
                func_800C54F0(m->slot, 0);
                return;
            }
            if (D_8014A110 == 4 || D_8014A110 == 5 || D_8014A110 == 6) {
                continue;
            }
            if ((D_801174B4 & 0x400008) && D_8014A110 != 1 && !D_801403D0 && !D_80152718 && !D_8013FECB &&
                gc->place_locked != 1 && func_800CF604(m->slot) && m->drone_scale > 0.1f &&
                (m->magvel < 25.0f || abort_me)) {
                if (!abort_me) {
                    if (m->stuck_time == 0.0f) {
                        m->stuck_time = m->time;
                    } else {
                        wait_time = 1;
                        if (D_8014A118[node].unk8 & D_8014A118[node].unk2C) {
                            wait_time = 2;
                        }
                        if (wait_time != 4) {
                            if (m->time - m->stuck_time > 6.0f * wait_time) {
                                abort_me = 1;
                            }
                        } else {
                            m->stuck_time = m->time;
                        }
                    }
                }
                if (abort_me) {
                    func_800C4F68(2, m, 1);
                    gc->we_died = 2;
                    gc->unk359 = 0xFF;
                    m->stuck_time = 0.0f;
                    m->unk6CC = 1;
                }
            } else {
                m->stuck_time = 0.0f;
            }
        }
    }

    for (node = 0; node < D_8014A108; node++) {
        m = &D_8014A250[D_8014A118[node].node];
        gc = &D_80152818[D_8014A118[node].node];
        if (gc->last_distance < gc->distance) {
            gc->fwd_progress_t = 0.0f;
            gc->last_distance = gc->distance;
            gc->stray_angle = 0.0f;
        } else if (!func_800CF604(m->slot)) {
            gc->fwd_progress_t = 0.0f;
            gc->stray_angle = 0.0f;
        } else if (gc->fwd_progress_t == 0.0f) {
            gc->fwd_progress_t = m->time;
        } else if (m->time - gc->fwd_progress_t > 2.0f) {
            idx = gc->mpi;
            seg = gc->seg;
            for (i = 0; i < 5; i++) {
                func_800B9F60(idx, seg, &idx, &seg);
            }
            if (idx >= 0) {
                delta[0] = D_801407F0.branch[idx].pts[seg][0] - gc->dr_pos[0];
                delta[1] = D_801407F0.branch[idx].pts[seg][1] - gc->dr_pos[1];
                delta[2] = D_801407F0.branch[idx].pts[seg][2] - gc->dr_pos[2];
            } else {
                delta[0] = D_801407F0.pts[seg][0] - gc->dr_pos[0];
                delta[1] = D_801407F0.pts[seg][1] - gc->dr_pos[1];
                delta[2] = D_801407F0.pts[seg][2] - gc->dr_pos[2];
            }
            func_800A61B0(delta, pos2, gc->dr_uvs);
            gc->stray_angle = func_8008C768(pos2[0], pos2[2]);
        }
    }
}
