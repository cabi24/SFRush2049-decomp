/* flags: -g0 -O3 -mips2 -G 0 -non_shared  (whole-program group; keep = func_800E3724) */
/*
 * func_800E3724 = N64 descendant of arcade sym()      (reference/repos/rushtherock/game/drivsym.c)
 * func_800E3430 = N64 descendant of arcade controls() (reference/repos/rushtherock/game/controls.c)
 * Proven from the bodies: the bog-state test `autotrans == 0 && throttle > .5 && abs(rpm) < 600`,
 * the peak_center_force / peak_body_force loops, the crashflag reset, and in controls() the
 * dt/idt/steerangle prologue, the place_locked / coast branches, the `brake > .99` clamp, the
 * commandgear/gear assignment and the brake-torque rolloff loop.  N64 differences: inputs are
 * floats (no CTLSCALE), the pow() brake ramps are constants 0.7 / 0.33, rolloff below 2 rad/s is 0
 * and the slope is 0.05, COUNTDOWN is the sign bit of the flags word at +0x710, mcommunication()
 * is reduced to the rpm line and written in place, and sym() ends by moving CENTERFORCE to +0x130
 * and clearing it and +0x13C.
 *
 * func_800E3430 is internal: it uses $f20-$f26 without saving them and the caller saves them,
 * and the caller keeps `m` in $a3 across the call.  It only reproduces with func_800E3724 kept.
 *
 * State (tools/cloud/score.py group): both members print MATCH with only their own float
 * literals unverified (.rodata+0x0..0xC = 0.7, 0.33, 0.99, 0.05 for func_800E3430 at retail
 * 0x801243F0..FC; .rodata+0x10/0x14 = 9.5493, 0.9 for func_800E3724 at 0x80124400/04).
 * Not strict, so "claims" is empty.  Spelling the literals as extern D_801243Fx changes the code
 * of func_800E3430 (the 0.05 load is inside the loop; the others re-colour $f0/$f2).
 *
 * Shaping that matters:
 *  - `m->idt = 1 / m->dt;` integer 1: with 1.0f the constant shares the later 1.0f web and is
 *    hoisted into $f2 at the top.
 *  - brake is assigned before throttle/clutch in the two locked branches (arcade order).
 *  - abs is spelled `(x >= 0) ? x : -x`.
 *  - the last three CENTERFORCE copies are written before the chained zero stores.
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

