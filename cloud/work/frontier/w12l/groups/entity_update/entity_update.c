/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * entity_update (0x800C6AA0, 391 words) -- tire surface probe for one wheel.
 * Arcade ancestor: game/stree.c tiresurf(m, ipos, opos, roadcode, uvs, whl): walk the quadtree
 * leaf from the wheel's last node, scan the poly list, keep the best surface, set
 * *surf = flags & 0xF; on a miss opos = ipos with the default plane 200 below and uvs = identity.
 * N64 additions: the per-wheel hint (+1440), the 0x2000/0x1000 snap handlers and the 3/4/7
 * surface effects gated on game_car[car->id] (D_80152818, CAR_DATA, 952 bytes) state bytes.
 * Whole-program -O3 unit (blob_unit EQUAL, 0 locked bodies differ).  History: w10c 27 -> w11b 10 -> w12a 0.
 * Shaping quirks (disclosed):
 *  1. eu_nop(): an empty static helper called after func_800B61A8(21,0,1,2).  umerge 8-aligns
 *     each inlined callee's frame area in call-site order; this trailing inlined call aligns the
 *     spill-temp area start 204 -> 208, which retail needs (w11b).  Probably a stubbed-out N64
 *     call; it leaves no code and no stub in the unit.
 *  2. `if (D_80152818[car->id].place) {}`: a compiled-out (debug) read of the game_car record.
 *     It emits no code but adds one use to the slot-address PRE web, raising its uopt p1
 *     priority (save) above the *surf reload web (both were 1.5; the tie went to *surf by lower
 *     web number).  The slot address then takes v0 and *surf v1, as in retail (w12a).
 *     Any field the real code does not load works (dr_pos[0], pad bytes at 0/858; tested before
 *     or inside the state test, or in the surf==4 arm); a read of b856/b857 CSEs with the real
 *     load and changes the code.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define NULL ((void *)0)
float fabsf(float);
#pragma intrinsic (fabsf)

typedef struct QNode {
    s16 next;
    u8 pad2;
    u8 mask;
    s16 x0;
    s16 x1;
    s16 y0;
    s16 y1;
    u16 child[4];
} QNode;
extern QNode *D_80124EEC;

typedef struct Poly {
    u16 type;
    u16 cnt;
    u8 body[0x12];
    u16 off;
} Poly;
extern Poly *D_801497F8;
extern u8 *D_80152460;

void *handbrake_apply(QNode *n, s16 x, s16 y, s16 *out);
void func_800ADCE0(u8 *input, s32 count, u16 *front, u32 marker);
s16 func_800C3AD0(Poly *poly, f32 *wp, f32 *q, f32 *pt, f32 *bound, s16 *outIdx, f32 *mat, f32 zmin);
void math_utility(void *, void *);
void steering_sensitivity(s32 arg0, u16 idx, f32 *position, f32 *outPosition, f32 (*outMatrix)[3], f32 threshold);
void traction_control(s32 arg0, u16 idx, f32 *pos, f32 *outPos, f32 (*outMat)[3]);
f32 func_8008E0B8(f32 *v);

typedef struct EUCar {
    u8 pad0[928];
    QNode *node[4];
    u8 pad944[1440 - 944];
    u16 hint[4];
    u8 pad1448[1564 - 1448];
    s16 kind[4];
    s16 slope[4];
    u16 attr[4];
    u8 pad1588[1600 - 1588];
    u8 b1600;
    u8 pad1601[7];
    Poly *p1608;
    u8 pad1612[1732 - 1612];
    s16 s1732;
    u8 pad1734[1741 - 1734];
    u8 b1741;
    u8 pad1742[1990 - 1742];
    s16 id;
} EUCar;
typedef struct EUSlot {
    f32 dr_pos[3];          /* 0x000 arcade CAR_DATA dr_pos */
    u8 pad00C[856 - 12];
    s8 b856;
    s8 b857;                /* 0x359 state (battle_mode_setup) */
    u8 pad35A;
    s8 place;               /* 0x35B (func_800EC914) */
    u8 pad35C[952 - 860];
} EUSlot;
extern EUSlot D_80152818[];
extern s8 D_8010FFC0;
extern f32 D_8011418C[];
void effect_cleanup(s8 a, s8 b, s8 c);
void func_800C54F0(s16 arg0, s32 arg1);
u32 entity_flags_apply(u32 index, u32 other, u32 value, u8 mode);
s16 input_process_controller(f32 *p1, f32 *p2, f32 *out, Poly *poly, s16 *outIdx, s32 flag, f32 *vcOut, f32 *mat, f32 rad2);
s32 func_800B61A8(s32 arg0, s32 arg1, s32 arg2, unsigned char arg3);

static void eu_nop(void) {}
void entity_update(EUCar *car, f32 *pos, f32 *out, s32 *surf, f32 (*mat)[3], s32 wheel) {
    f32 m[3][3];
    u32 i;
    u8 *p;
    u16 idx;
    u16 bestIdx;
    s16 quad;
    u8 n;
    QNode *node;
    f32 h;
    u32 off;
    f32 pt[3];
    u16 list[36];
    f32 bestAbs;
    f32 bestH;
    Poly *poly;
    Poly *best;
    f32 x;
    f32 z;

    best = NULL;
    bestAbs = 250.0f;
    if (pos[0] < 0.0f && (f32)(s32)pos[0] != pos[0]) {
        x = pos[0] - 1.0f;
    } else {
        x = pos[0];
    }
    if (pos[2] < 0.0f && (f32)(s32)pos[2] != pos[2]) {
        z = pos[2] - 1.0f;
    } else {
        z = pos[2];
    }
    node = handbrake_apply(car->node[wheel], (s32)x, (s32)z, &quad);
    if (node == NULL) {
        goto miss;
    }
    car->node[wheel] = node;
    off = node->child[quad];
    if (node->mask & (16 << quad)) {
        off |= 0x10000;
    }
    if (off != 0) {
    p = D_80152460 + off;
    n = *p++;
    func_800ADCE0(p, n, list, car->hint[wheel]);
    for (i = 0; i < n; i++) {
        poly = &D_801497F8[list[i]];
        if ((poly->type & 0xF) == 5 || (poly->type & 0xF) == 6 || (poly->type & 0xF) == 15) {
            continue;
        }
        h = bestAbs;
        if (func_800C3AD0(poly, pos, NULL, pt, &h, (s16 *)&idx, (f32 *)m, -5.0f) != 0) {
            if (-5.0f < h && h < bestAbs) {
                best = poly;
                bestIdx = idx;
                bestAbs = h;
                out[0] = pt[0];
                out[1] = pt[1];
                out[2] = pt[2];
                math_utility(m, mat);
                car->hint[wheel] = list[i];
            }
        }
    }
    if (bestAbs != 250.0f) {
    if (out[1] < 1.0f && (best->cnt & 0x30)) {
        car->p1608 = best;
    }
    if (best->type & 0x2000) {
        steering_sensitivity((s32)best, bestIdx, pos, out, mat, 250.0f);
        if (out[1] < -5.0f) {
            goto miss;
        }
    } else if (best->type & 0x1000) {
        traction_control((s32)best, bestIdx, pos, out, mat);
    }
    *surf = best->type & 0xF;
    car->kind[wheel] = (best->type & 0xF0) >> 4;
    car->slope[wheel] = (best->type & 0xF00) >> 8;
    if (best->type & 0x8000) {
        car->slope[wheel] = -car->slope[wheel];
    }
    car->attr[wheel] = best->cnt;
    if (best->cnt & 0x30) {
        car->attr[wheel] &= 0x7FF;
    }
    if ((*surf == 3 || *surf == 4 || *surf == 7) && out[1] < 0.25f) {
        if (D_80152818[car->id].b857 == 0 && D_80152818[car->id].b856 == 0 && car->s1732 == -1) {
            if (D_80152818[car->id].place) {}
            if (*surf == 4) {
                effect_cleanup(car->id, car->id, -1);
                car->b1600 = 1;
            } else if (*surf == 7) {
                car->b1741 = 1;
                func_800C54F0(car->id, 1);
            } else {
                car->b1741 = 1;
                func_800C54F0(car->id, 1);
                func_800B61A8(21, 0, 1, 2);
                eu_nop();
            }
        }
    }
    return;
    }
    }
miss:
    *surf = 0;
    car->kind[wheel] = 0;
    car->slope[wheel] = 0;
    car->attr[wheel] = 0;
    out[0] = pos[0];
    out[1] = pos[1] - -200.0f;
    out[2] = pos[2];
    math_utility(D_8011418C, mat);
}
