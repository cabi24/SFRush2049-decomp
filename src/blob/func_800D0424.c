/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Arcade ancestor: positions(MODELDAT *m) in
 * reference/repos/rushtherock/game/drivsym.c (proven by structure: bodtorw of
 * A and V, RWR += RWV*dt, rotuv(W*dt), rwtobod, then the TIRER/BODYR
 * body-to-world loops). Callees: func_8009E820 = bodtorw, func_800A61B0 =
 * rwtobod, sound_position_set = rotuv (historical label), func_8008B424 =
 * reciprocal vector length, menu_video_settings = (historical label) the
 * alternative UV re-orthonormaliser.
 * N64 differences from the arcade body: the previous RWR is saved first;
 * scalmul/vecadd are open-coded; W*dt is also accumulated into m->angle when
 * D_8014A110 == 4; every 1024 frames the three UV rows are renormalised; the
 * suspension compressions tpcomp[] are clamped to 0.5 and the largest excess
 * is moved into the body position; TIRER comes from the car-type record at
 * m->car and tpcomp is added to its y component; no thetime update.
 * MODELDAT offsets here are N64 offsets recovered from this function only.
 *
 * Shaping quirks (all needed for MATCH):
 *  - the lastRWR copy is one comma expression (three separate statements load
 *    and store in the opposite order / with the opposite temp numbering);
 *  - operand order of the sums as written (`temp + RWR`, `rwp + RWR`,
 *    `bp[1] + tpcomp`);
 *  - the unused arcade locals stay declared: they supply the 176-byte frame
 *    (arcade's `moveup` and `ptch` are dropped; which two scalars the retail
 *    source dropped is not known). `movef` is reused as the scratch scalar.
 * Also MATCH at -O2 with the same source. Prior attempt:
 * cloud/work/near_miss_B15 (171/249).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef f32 F32;

#define XCOMP 0
#define YCOMP 1
#define ZCOMP 2

typedef struct {
    char pad0[0x70];
    F32 TIRER[4][3];        /* 0x70 */
} CARTYPE;

typedef struct {
    F32 fpuvs[3][3];
} UVECT;

typedef struct MODELDAT {
    CARTYPE *car;           /* 0x000 */
    char pad4[0x24];
    F32 A[3];               /* 0x028 */
    char pad34[0xC];
    F32 V[3];               /* 0x040 */
    F32 W[3];               /* 0x04C */
    char pad58[0x9C];
    F32 BODYR[4][3];        /* 0x0F4 */
    char pad124[0xF0];
    F32 RWA[3];             /* 0x214 */
    F32 RWV[3];             /* 0x220 */
    F32 RWR[3];             /* 0x22C */
    F32 lastRWR[3];         /* 0x238 */
    F32 TIRERWR[4][3];      /* 0x244 */
    F32 BODYRWR[4][3];      /* 0x274 */
    char pad2A4[0x3C];
    F32 angle[3];           /* 0x2E0 */
    UVECT UV;               /* 0x2EC */
    char pad310[0x2CC];
    F32 tpcomp[4];          /* 0x5DC */
    char pad5EC[0x48];
    F32 dt;                 /* 0x634 */
    char pad638[0xD8];
    s32 frame;              /* 0x710 */
} MODELDAT;

extern s32 D_8014A110;
extern s8 D_801427A1;

void func_8009E820(f32 *arg0, f32 *arg1, f32 *arg2);
void func_800A61B0(f32 *arg0, f32 *arg1, f32 *arg2);
void sound_position_set(void *arg0, void *arg1);
f32 func_8008B424(f32 *v);
void menu_video_settings(void *arg0);

void func_800D0424(MODELDAT *m) {
    F32 temp[3], temp1[3], temp2[3], *bp, *rwp;
    F32 movef, mover;
    F32 yvect[3], ftroll[3], rtroll[3];
    F32 rollang;
    s32 i;

    m->lastRWR[0] = m->RWR[0], m->lastRWR[1] = m->RWR[1], m->lastRWR[2] = m->RWR[2];

    func_8009E820(m->A, m->RWA, m->UV.fpuvs[0]);
    func_8009E820(m->V, m->RWV, m->UV.fpuvs[0]);

    temp[0] = m->RWV[0] * m->dt;
    temp[1] = m->RWV[1] * m->dt;
    temp[2] = m->RWV[2] * m->dt;

    m->RWR[0] = temp[0] + m->RWR[0];
    m->RWR[1] = temp[1] + m->RWR[1];
    m->RWR[2] = temp[2] + m->RWR[2];

    temp[XCOMP] = m->W[XCOMP] * m->dt;
    temp[YCOMP] = m->W[YCOMP] * m->dt;
    temp[ZCOMP] = m->W[ZCOMP] * m->dt;

    if (D_8014A110 == 4) {
        m->angle[0] = temp[0] + m->angle[0];
        m->angle[1] = temp[1] + m->angle[1];
        m->angle[2] = temp[2] + m->angle[2];
    }

    sound_position_set(temp, &m->UV);

    if ((m->frame & 0x3FF) == 0x3FF) {
        if (D_801427A1 != 0) {
            menu_video_settings(m);
        } else {
            movef = func_8008B424(m->UV.fpuvs[0]);
            m->UV.fpuvs[0][0] *= movef;
            m->UV.fpuvs[0][1] *= movef;
            m->UV.fpuvs[0][2] *= movef;
            movef = func_8008B424(m->UV.fpuvs[1]);
            m->UV.fpuvs[1][0] *= movef;
            m->UV.fpuvs[1][1] *= movef;
            m->UV.fpuvs[1][2] *= movef;
            movef = func_8008B424(m->UV.fpuvs[2]);
            m->UV.fpuvs[2][0] *= movef;
            m->UV.fpuvs[2][1] *= movef;
            m->UV.fpuvs[2][2] *= movef;
        }
    }

    func_800A61B0(m->RWV, m->V, m->UV.fpuvs[0]);

    temp[1] = 0.0f;
    for (i = 0; i < 4; i++) {
        movef = m->tpcomp[i] - 0.5f;
        if (temp[1] < movef) {
            temp[1] = movef;
            m->tpcomp[i] = 0.5f;
        }
    }
    if (temp[1] > 0.0f) {
        temp[0] = 0.0f;
        temp[2] = 0.0f;
        func_8009E820(temp, temp1, m->UV.fpuvs[0]);
        m->RWR[0] = temp1[0] + m->RWR[0];
        m->RWR[1] = temp1[1] + m->RWR[1];
        m->RWR[2] = temp1[2] + m->RWR[2];
    }

    for (i = 0; i < 4; i++) {
        rwp = m->TIRERWR[i];
        bp = m->car->TIRER[i];
        temp[0] = bp[0];
        temp[1] = bp[1] + m->tpcomp[i];
        temp[2] = bp[2];
        func_8009E820(temp, rwp, m->UV.fpuvs[0]);
        rwp[0] = rwp[0] + m->RWR[0];
        rwp[1] = rwp[1] + m->RWR[1];
        rwp[2] = rwp[2] + m->RWR[2];
    }

    for (i = 0; i < 4; i++) {
        bp = m->BODYR[i];
        rwp = m->BODYRWR[i];
        func_8009E820(bp, rwp, m->UV.fpuvs[0]);
        rwp[0] = rwp[0] + m->RWR[0];
        rwp[1] = rwp[1] + m->RWR[1];
        rwp[2] = rwp[2] + m->RWR[2];
    }
}
