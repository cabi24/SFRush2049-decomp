typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef float f32;

typedef struct {
    char pad0[0x124];
    f32 CENTERFORCE[3];         /* 0x124 */
    char pad130[0x359 - 0x130];
    s8 unk359;
    char pad35A[0x3B8 - 0x35A];
} Car;

typedef struct {
    char pad0[0x124];
    f32 CENTERFORCE[3];         /* 0x124 */
    char pad130[0x5C4 - 0x130];
    f32 mass;                   /* 0x5C4 */
    char pad5C8[0x63C - 0x5C8];
    f32 unk63C;                 /* 0x63C */
    s8 unk640;                  /* 0x640 */
    char pad641[0x788 - 0x641];
    f32 RWV[3];                 /* 0x788 */
    f32 RWR[3];                 /* 0x794 */
    f32 UV[3][3];               /* 0x7A0 */
    char pad7C4[2];
    s16 net_node;               /* 0x7C6 */
    char pad7C8[4];
    s8 unk7CC;                  /* 0x7CC */
} MODELDAT;

typedef struct {
    f32 BODYR[4];               /* x min, x max, z max, z min */
    char pad10[0xC];
    f32 mass;                   /* 0x1C */
} Body;

typedef struct {
    Body *body;                 /* 0x00 */
    f32 UV[3][3];               /* 0x04 */
    f32 RWR[3];                 /* 0x28 */
    f32 RWV[3];                 /* 0x34 */
} Reckon;

extern s8 D_8017A634;
extern s32 D_8014A110;
extern Car D_80152818[];

extern f32 fabsf(f32);
#pragma intrinsic(fabsf)

void func_800A61B0(f32 *arg0, f32 *arg1, f32 *arg2);
void func_8009E820(f32 *arg0, f32 *arg1, f32 *arg2);
f32 func_8008B3C8(f32 *v);
void random_int(MODELDAT *car, f32 *dir);

void random_float(s32 arg0, MODELDAT *m, Reckon *m2, f32 *dir, f32 *pos) {
    f32 force[3], temp[3], unused[3], rvel[3], cent[3];
    f32 mass;
    s32 i;
    s32 behind;
    s32 pad1;
    s32 pad2;
    s32 beside;

    temp[0] = m->RWR[0] - m2->RWR[0];
    temp[1] = m->RWR[1] - m2->RWR[1];
    temp[2] = m->RWR[2] - m2->RWR[2];
    func_800A61B0(temp, cent, m2->UV[0]);
    behind = ((cent[2] < m2->body->BODYR[2]) && (cent[2] > m2->body->BODYR[3]));
    beside = ((cent[0] < m2->body->BODYR[1]) && (cent[0] > m2->body->BODYR[0]));
    if (behind && beside && m->unk640) {
        random_int(m, dir);
        return;
    }
    temp[0] = m->RWV[0] - m2->RWV[0];
    temp[1] = m->RWV[1] - m2->RWV[1];
    temp[2] = m->RWV[2] - m2->RWV[2];
    func_800A61B0(temp, rvel, m2->UV[0]);

    if (behind && !beside) {
        force[2] = 0.0;
    } else {
        force[2] = rvel[2] * 2500.0f;
        if (pos[2] > 0.0f) {
            if (force[2] > -4000.0f) {
                force[2] = -4000.0f;
            }
            if (beside) {
                temp[2] = pos[2] - m2->body->BODYR[2];
                temp[2] = temp[2] * (fabsf(temp[2]) * 10000.0f);
                if (temp[2] < force[2]) {
                    force[2] = temp[2];
                }
            }
        } else if (pos[2] < 0.0f) {
            if (force[2] < 4000.0f) {
                force[2] = 4000.0f;
            }
            if (beside) {
                temp[2] = pos[2] - m2->body->BODYR[3];
                temp[2] = temp[2] * (fabsf(temp[2]) * 10000.0f);
                if (temp[2] > force[2]) {
                    force[2] = temp[2];
                }
            }
        }
    }

    if (beside && !behind) {
        force[0] = 0.0;
    } else {
        force[0] = rvel[0] * 2500.0f;
        if (pos[0] > 0.0f) {
            if (force[0] > -4000.0f) {
                force[0] = -4000.0f;
            }
            if (behind) {
                temp[0] = pos[0] - m2->body->BODYR[1];
                temp[0] = temp[0] * (fabsf(temp[0]) * 20000.0f);
                if (temp[0] < force[0]) {
                    force[0] = temp[0];
                }
            }
        } else if (pos[0] < 0.0f) {
            if (force[0] < 4000.0f) {
                force[0] = 4000.0f;
            }
            if (behind) {
                temp[0] = pos[0] - m2->body->BODYR[0];
                temp[0] = temp[0] * (fabsf(temp[0]) * 20000.0f);
                if (temp[0] > force[0]) {
                    force[0] = temp[0];
                }
            }
        }
    }

    force[1] = rvel[1] * 100.0f;
    mass = (m->mass + m2->body->mass) * 0.5f;
    func_8009E820(force, temp, m2->UV[0]);
    func_800A61B0(temp, cent, m->UV[0]);
    mass = m2->body->mass / mass;
    cent[0] *= mass;
    cent[1] *= mass;
    cent[2] *= mass;
    m->CENTERFORCE[0] -= cent[0];
    m->CENTERFORCE[1] -= cent[1];
    m->CENTERFORCE[2] -= cent[2];
    i = (m->unk7CC == 2);
    if (D_8017A634 == 1 || (D_8017A634 == 2 && i) || func_8008B3C8(cent) > m->unk63C * 0.8f) {
        if (D_8014A110 != 6 && m->unk640 == 0 && D_80152818[m->net_node].unk359 != 2) {
            m->unk640 = 1;
        }
    }
}
