/* flags: -g0 -O3 -mips2 -G 0 -non_shared  (whole-program group; keep = func_800E3724) */
/*
 * Variant of ../sym_controls/group.c: identical except that the two float literals of
 * func_800E3724 (9.5493f, 0.9f) are spelled `extern const F32 D_80124400 / D_80124404`, so that
 * the scorer can check their addresses.  func_800E3724 then prints strict MATCH with unchanged
 * code.  func_800E3430 keeps natural literals (externs change its code) and stays
 * "MATCH (8 section-relative relocations unverified)".  See ../sym_controls/group.c for semantics.
 */
typedef signed char S8;
typedef unsigned char U8;
typedef signed short S16;
typedef signed int S32;
typedef float F32;

typedef struct tiredes {
    /* 0x00 */ U8 pad00[0x48];
    /* 0x48 */ F32 angvel;
    /* 0x4C */ U8 pad4C[0x10];
} tiredes;

typedef struct MODELDAT {
    /* 0x000 */ U8 pad000[10];
    /* 0x00A */ S8 autotrans;
    /* 0x00B */ U8 pad00B[0xC4 - 0xB];
    /* 0x0C4 */ F32 BODYFORCE[4][3];
    /* 0x0F4 */ U8 pad0F4[0x124 - 0xF4];
    /* 0x124 */ F32 CENTERFORCE[3];
    /* 0x130 */ F32 lastforce[3];
    /* 0x13C */ F32 unk13C[3];
    /* 0x148 */ F32 peak_body_force[2][3];
    /* 0x160 */ F32 peak_center_force[2][3];
    /* 0x178 */ U8 pad178[0x3B0 - 0x178];
    /* 0x3B0 */ F32 steerangle;
    /* 0x3B4 */ F32 torque[4];
    /* 0x3C4 */ F32 steergain;
    /* 0x3C8 */ F32 pad3C8;
    /* 0x3CC */ F32 clutch;
    /* 0x3D0 */ F32 throttle;
    /* 0x3D4 */ F32 brake;
    /* 0x3D8 */ F32 pad3D8;
    /* 0x3DC */ F32 brakegain[4];
    /* 0x3EC */ U8 pad3EC[8];
    /* 0x3F4 */ S16 gear;
    /* 0x3F6 */ S16 commandgear;
    /* 0x3F8 */ U8 pad3F8[0x408 - 0x3F8];
    /* 0x408 */ F32 engangvel;
    /* 0x40C */ U8 pad40C[0x430 - 0x40C];
    /* 0x430 */ tiredes tires[4];
    /* 0x5A0 */ U8 pad5A0[0x634 - 0x5A0];
    /* 0x634 */ F32 dt;
    /* 0x638 */ F32 idt;
    /* 0x63C */ U8 pad63C[4];
    /* 0x640 */ S8 crashflag;
    /* 0x641 */ U8 pad641[0x65C - 0x641];
    /* 0x65C */ S16 bog_state;
    /* 0x65E */ U8 pad65E[0x710 - 0x65E];
    /* 0x710 */ S32 flags;
    /* 0x714 */ U8 pad714[4];
    /* 0x718 */ F32 in_modeltime;
    /* 0x71C */ U8 pad71C[4];
    /* 0x720 */ F32 in_wheel;
    /* 0x724 */ F32 in_clutch;
    /* 0x728 */ F32 in_brake;
    /* 0x72C */ F32 in_throttle;
    /* 0x730 */ S8 in_gear;
    /* 0x731 */ U8 pad731[0x7C6 - 0x731];
    /* 0x7C6 */ S16 net_node;
    /* 0x7C8 */ U8 pad7C8[4];
    /* 0x7CC */ S8 mode;
    /* 0x7CD */ U8 pad7CD[3];
    /* 0x7D0 */ S16 rpm;
} MODELDAT;

typedef struct CAR_DATA {
    /* 0x000 */ U8 pad000[0xEF];
    /* 0x0EF */ S8 place_locked;
    /* 0x0F0 */ U8 pad0F0[0x359 - 0xF0];
    /* 0x359 */ S8 unk359;
    /* 0x35A */ U8 pad35A[0x3B8 - 0x35A];
} CAR_DATA;

extern CAR_DATA player_array[];
extern S8 D_80152718;
extern S8 D_8013FECB;
extern const F32 D_80124400;
extern const F32 D_80124404;
extern void func_800E32CC(MODELDAT *m);
extern void object_update_full(MODELDAT *m);
extern void func_800E0B20(MODELDAT *m);

void func_800E3430(MODELDAT *m) {
    F32 rolloff;
    S32 i;
    tiredes *td;
    CAR_DATA *gc = &player_array[m->net_node];

    m->dt = m->in_modeltime;
    m->idt = 1 / m->dt;
    m->steerangle = m->steergain * m->in_wheel;
    m->clutch = m->in_clutch;
    if (gc->place_locked == 1) {
        m->throttle = 0.0f;
    } else {
        m->throttle = m->in_throttle;
    }
    if (D_80152718 || gc->place_locked) {
        m->brake = 0.7f;
        m->throttle = 0.0f;
        m->clutch = 1.0f;
    } else if (D_8013FECB) {
        m->brake = 0.33f;
        m->throttle = 0.0f;
        m->clutch = 1.0f;
    } else {
        m->brake = m->in_brake;
    }
    if (m->brake > 0.99f) {
        m->brake = 1.0f;
    }
    if (!m->autotrans) {
        m->commandgear = m->gear = m->in_gear;
    } else {
        m->commandgear = m->in_gear;
    }
    if (m->flags < 0) {
        m->commandgear = 0;
        m->gear = 0;
        m->clutch = 1.0f;
        m->brake = 1.0f;
    }
    for (i = 0, td = &m->tires[0]; i < 4; ++i, ++td) {
        if (td->angvel > 0.0f) {
            if (td->angvel < 10.0f) {
                if (td->angvel < 2.0f) {
                    rolloff = 0.0f;
                } else {
                    rolloff = td->angvel * 0.05f;
                }
            } else {
                rolloff = 1.0f;
            }
        } else {
            if (td->angvel > -10.0f) {
                if (td->angvel > -2.0f) {
                    rolloff = 0.0f;
                } else {
                    rolloff = td->angvel * 0.05f;
                }
            } else {
                rolloff = -1.0f;
            }
        }
        m->torque[i] = -m->brakegain[i] * m->brake * rolloff;
    }
}

void func_800E3724(MODELDAT *m) {
    S32 absvel, i, j;

    func_800E3430(m);
    if (m->crashflag || player_array[m->net_node].unk359) {
        m->steerangle = 0.0f;
        m->clutch = 0.0f;
        m->brake = 0.0f;
        m->throttle = 0.0f;
    }
    func_800E32CC(m);
    object_update_full(m);
    func_800E0B20(m);
    m->rpm = (S16) (m->engangvel * D_80124400 * D_80124404);
    if ((m->autotrans == 0) && (m->throttle > 0.5f) && (((m->rpm >= 0) ? m->rpm : -m->rpm) < 600)) {
        if (m->bog_state == 0) {
            m->bog_state = 1;
        }
    } else {
        if (m->bog_state) {
            m->bog_state = 3;
        }
    }
    if (m->mode == 2) {
        for (i = 0; i < 3; i++) {
            if (m->CENTERFORCE[i] > m->peak_center_force[0][i]) {
                m->peak_center_force[0][i] = m->CENTERFORCE[i];
            }
            if (m->CENTERFORCE[i] < m->peak_center_force[1][i]) {
                m->peak_center_force[1][i] = m->CENTERFORCE[i];
            }
            for (j = 0; j < 4; j++) {
                if (m->BODYFORCE[j][i] > m->peak_body_force[0][i]) {
                    m->peak_body_force[0][i] = m->BODYFORCE[j][i];
                }
                if (m->BODYFORCE[j][i] < m->peak_body_force[1][i]) {
                    m->peak_body_force[1][i] = m->BODYFORCE[j][i];
                }
            }
        }
    }
    m->lastforce[0] = m->CENTERFORCE[0];
    m->lastforce[1] = m->CENTERFORCE[1];
    m->lastforce[2] = m->CENTERFORCE[2];
    m->CENTERFORCE[0] = m->CENTERFORCE[1] = m->CENTERFORCE[2] = 0.0f;
    m->unk13C[0] = 0.0f;
    m->unk13C[1] = 0.0f;
    m->unk13C[2] = 0.0f;
}
