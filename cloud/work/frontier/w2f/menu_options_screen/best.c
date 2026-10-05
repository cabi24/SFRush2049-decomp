/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * menu_options_screen (historical label) @ 0x800CE358, 2,356 bytes: car-to-car collision force.
 * N64 descendant of arcade setFBCollisionForce (reference/repos/rushtherock/game/collision.c):
 * m1 has a corner inside m2's box, `pos` is that corner in m2's frame. Computes whether m1's
 * centre is beside/behind m2, calls ForceApart (label menu_load_options) on a major overlap,
 * otherwise builds a force in m2's frame from the relative velocity (2000 per unit, clamped to
 * +-4000 by the corner's side, plus a 20000*d*|d| penetration term when beside), rotates it to
 * world and to m1's frame (func_8009E820 = bodtorw, func_800A61B0 = rwtobod) and applies it to
 * both cars split by mass. N64 additions: the
 * `collidable` hand-off flags at the top (car +0x35F, model +0x640, velocity halved), a battle-mode
 * (gameplay_mode == 6) hit rule that takes a life (car +0x385) and calls func_8038D3A4(car, other,
 * time) for a rear hit of more than 2.25 units, and the crash flag at model +0x640 when the
 * applied force exceeds half of model +0x63C (func_8008B3C8 = vector magnitude).
 * Axis order differs from the arcade: BODYR column 2 is front/back here, column 0 is the side.
 *
 * State: code identical (589/589 words; EQUAL in the whole-program unit), own-rodata unverified
 * by the unpatched scorer: four literals at 0x80124108 (3EAAAAAA 3EAAAAAA 469C4000 469C4000),
 * checked by hand against the compiled .rodata. Needs -O3 (-O2: s-registers, 582 words differ).
 *
 * Shaping facts (each needed, found by removing it):
 * - `time = 160; time += x * 7.0f;` with an int variable: the compound assignment keeps the
 *   constant as the left operand of add.s (any `160.0f + x * 7.0f` expression puts it right);
 * - `k = 20000; temp[0] *= k * fabsf(temp[0]);` for the same reason (mul.s K, |d|);
 * - `m2->CENTERFORCE[i] = m2->CENTERFORCE[i] + force[i]` written out: IDO reverses the operands
 *   of a plain binary add, `+=` does not;
 * - int `0` for the two force stores and for `unk3AC <= 0`, `0.0f` for the pos comparisons
 *   (different constants: retail shares one zero register only among the pos comparisons);
 * - the timer store before the life decrement (`unk3AC = ...; unk385--;`);
 * - `f32 unused[3]` between temp and rvel (12 frame bytes), declaration order as below.
 */
typedef float f32;
typedef int s32;
typedef signed char s8;
typedef signed short s16;

typedef struct Model {
    /* 0x000 */ char pad0[0xF4];
    /* 0x0F4 */ f32 BODYR[4][3];
    /* 0x124 */ f32 CENTERFORCE[3];
    /* 0x130 */ char pad130[0xF0];
    /* 0x220 */ f32 unk220[3];
    /* 0x22C */ char pad22C[0x1C4];
    /* 0x3F0 */ f32 unk3F0;
    /* 0x3F4 */ char pad3F4[0x1D0];
    /* 0x5C4 */ f32 mass;
    /* 0x5C8 */ char pad5C8[0x74];
    /* 0x63C */ f32 unk63C;
    /* 0x640 */ s8 unk640;
    /* 0x641 */ char pad641[0x83];
    /* 0x6C4 */ s16 unk6C4;
    /* 0x6C6 */ char pad6C6[0xC2];
    /* 0x788 */ f32 RWV[3];
    /* 0x794 */ f32 RWR[3];
    /* 0x7A0 */ f32 UV[3][3];
    /* 0x7C4 */ s16 pad7C4;
    /* 0x7C6 */ s16 slot;
    /* 0x7C8 */ char pad7C8[4];
    /* 0x7CC */ s8 unk7CC;
} Model;

typedef struct Car {
    /* 0x000 */ char pad0[0x358];
    /* 0x358 */ s8 unk358;
    /* 0x359 */ s8 unk359;
    /* 0x35A */ s8 unk35A;
    /* 0x35B */ s8 unk35B;
    /* 0x35C */ char pad35C[3];
    /* 0x35F */ s8 unk35F;
    /* 0x360 */ char pad360[0x24];
    /* 0x384 */ s8 unk384;
    /* 0x385 */ s8 unk385;
    /* 0x386 */ char pad386[0x26];
    /* 0x3AC */ f32 unk3AC;
    /* 0x3B0 */ char pad3B0[8];
} Car;

extern Car player_array[];
extern s32 gameplay_mode;
extern s8 D_8012E67C[];
extern s8 D_8017A634;

void func_800A61B0(f32 *arg0, f32 *arg1, f32 *arg2);
void func_8009E820(f32 *arg0, f32 *arg1, f32 *arg2);
f32 func_8008B3C8(f32 *v);
void menu_load_options(Model *car, Model *a, Model *b, f32 *dir);
void func_8038D3A4(Car *a, Car *b, s32 c);
f32 fabsf(f32);
#pragma intrinsic(fabsf)

void menu_options_screen(Model *m, Model *m1, Model *m2, f32 *dir, f32 *pos) {
    f32 force[3];
    f32 temp[3];
    f32 unused[3];
    f32 rvel[3];
    f32 cent[3];
    f32 cent2[3];
    f32 mass;
    f32 scale;
    s32 beside;
    s32 behind;
    s32 flag;
    s32 k;
    s32 time;
    Car *c1;
    Car *c2;

    c1 = &player_array[m1->slot];
    c2 = &player_array[m2->slot];
    if (m1->slot >= 0 && m2->slot >= 0 && gameplay_mode != 6) {
        if (c1->unk35F) {
            m2->unk640 = 1;
            c1->unk35F = 0;
            m2->unk220[0] *= 0.5f;
            m2->unk220[1] *= 0.5f;
            m2->unk220[2] *= 0.5f;
        }
        if (c2->unk35F) {
            m1->unk640 = 1;
            c2->unk35F = 0;
            m1->unk220[0] *= 0.5f;
            m1->unk220[1] *= 0.5f;
            m1->unk220[2] *= 0.5f;
        }
    }

    temp[0] = m1->RWR[0] - m2->RWR[0];
    temp[1] = m1->RWR[1] - m2->RWR[1];
    temp[2] = m1->RWR[2] - m2->RWR[2];
    func_800A61B0(temp, cent, m2->UV[0]);
    beside = (cent[2] < m2->BODYR[0][2]) && (cent[2] > m2->BODYR[3][2]);
    behind = (cent[0] < m2->BODYR[0][0]) && (cent[0] > m2->BODYR[3][0]);

    if (gameplay_mode == 6 && D_8012E67C[c1->unk35B] != D_8012E67C[c2->unk35B]) {
        if (c1->unk384 == 5 && m2->unk6C4 == -1 && c1->unk3AC <= 0 && c2->unk358 == 0) {
            temp[0] = m2->RWR[0] - m1->RWR[0];
            temp[1] = m2->RWR[1] - m1->RWR[1];
            temp[2] = m2->RWR[2] - m1->RWR[2];
            func_800A61B0(temp, cent2, m1->UV[0]);
            if (cent2[2] > 2.25f) {
                c1->unk3AC = 0.3333333f;
                c1->unk385--;
                time = 160;
                time += m1->unk3F0 * 7.0f;
                func_8038D3A4(c1, c2, time);
            }
        } else if (c2->unk384 == 5 && m1->unk6C4 == -1 && c2->unk3AC <= 0 && c1->unk358 == 0) {
            if (cent[2] > 2.25f) {
                c2->unk3AC = 0.3333333f;
                c2->unk385--;
                time = 160;
                time += m2->unk3F0 * 7.0f;
                func_8038D3A4(c2, c1, time);
            }
        }
    }

    if (beside && behind) {
        menu_load_options(m, m1, m2, dir);
        return;
    }

    temp[0] = m1->RWV[0] - m2->RWV[0];
    temp[1] = m1->RWV[1] - m2->RWV[1];
    temp[2] = m1->RWV[2] - m2->RWV[2];
    func_800A61B0(temp, rvel, m2->UV[0]);

    if (beside) {
        force[2] = 0;
    } else {
        force[2] = rvel[2] * 2000.0f;
        if (pos[2] > 0.0f) {
            if (force[2] > -4000.0f) {
                force[2] = -4000.0f;
            }
        } else if (pos[2] < 0.0f) {
            if (force[2] < 4000.0f) {
                force[2] = 4000.0f;
            }
        }
    }

    if (behind) {
        force[0] = 0;
    } else {
        force[0] = rvel[0] * 2000.0f;
        if (pos[0] > 0.0f) {
            if (force[0] > -4000.0f) {
                force[0] = -4000.0f;
            }
            if (beside) {
                temp[0] = pos[0] - m2->BODYR[2][0];
                k = 20000;
                temp[0] *= k * fabsf(temp[0]);
                if (temp[0] < force[0]) {
                    force[0] = temp[0];
                }
            }
        } else if (pos[0] < 0.0f) {
            if (force[0] < 4000.0f) {
                force[0] = 4000.0f;
            }
            if (beside) {
                temp[0] = pos[0] - m2->BODYR[1][0];
                k = 20000;
                temp[0] *= k * fabsf(temp[0]);
                if (temp[0] > force[0]) {
                    force[0] = temp[0];
                }
            }
        }
    }

    force[1] = rvel[1] * 100.0f;
    mass = (m1->mass + m2->mass) * 0.5f;

    func_8009E820(force, temp, m2->UV[0]);
    func_800A61B0(temp, cent, m1->UV[0]);
    scale = m2->mass / mass;
    cent[0] *= scale;
    cent[1] *= scale;
    cent[2] *= scale;
    m1->CENTERFORCE[0] -= cent[0];
    m1->CENTERFORCE[1] -= cent[1];
    m1->CENTERFORCE[2] -= cent[2];
    flag = (m1->unk7CC == 2) || (m2->unk7CC == 2);
    if (D_8017A634 == 1 || (D_8017A634 == 2 && flag) || func_8008B3C8(cent) > m1->unk63C * 0.5f) {
        if (gameplay_mode != 6 && m1->unk640 == 0 && c1->unk359 != 2) {
            m1->unk640 = 1;
        }
    }

    scale = m1->mass / mass;
    force[0] *= scale;
    force[1] *= scale;
    force[2] *= scale;
    m2->CENTERFORCE[0] = m2->CENTERFORCE[0] + force[0];
    m2->CENTERFORCE[1] = m2->CENTERFORCE[1] + force[1];
    m2->CENTERFORCE[2] = m2->CENTERFORCE[2] + force[2];
    if (D_8017A634 == 1 || (D_8017A634 == 2 && flag) || func_8008B3C8(force) > m2->unk63C * 0.5f) {
        if (gameplay_mode != 6 && m2->unk640 == 0 && c2->unk359 != 2) {
            m2->unk640 = 1;
        }
    }
}
