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
    u8 pad0[856];
    s8 b856;
    s8 b857;
    u8 pad858[952 - 858];
} EUSlot;
extern EUSlot D_80152818[];
extern s8 D_8010FFC0;
extern f32 D_8011418C[];
void effect_cleanup(s8 a, s8 b, s8 c);
void func_800C54F0(s16 arg0, s32 arg1);
u32 entity_flags_apply(u32 index, u32 other, u32 value, u8 mode);
s16 input_process_controller(f32 *p1, f32 *p2, f32 *out, Poly *poly, s16 *outIdx, s32 flag, f32 *vcOut, f32 *mat, f32 rad2);
s32 func_800B61A8(s32 arg0, s32 arg1, s32 arg2, unsigned char arg3);
/*
 * entity_update (0x800C6AA0, 391 words) -- w10c near-miss: 27/391 words, all of them
 * temp-slot numbers (+4: ours 112/116/120, retail 108/112/116) and one v0/v1 swap (*surf reload
 * vs. the D_80152818 slot pointer).  Opcode sequence identical (opdiff.py: 0 rows).
 * Per-wheel surface probe (sibling of camera_trigger_check; historical label):
 *   floor(pos) -> quadtree cell starting from car->node[wheel] (+928, cached back on success);
 *   polygon list (func_800ADCE0, marker = car->hint[wheel] +1440); func_800C3AD0 with zmin -5 and
 *   bound = best; keep -5 < h < best; out = hit point, mat = basis, hint[wheel] = poly index.
 *   Winner: (out.y < 1 && cnt & 0x30) -> car->p1608 = poly; 0x2000 -> steering_sensitivity
 *   (miss below -5), 0x1000 -> traction_control.  *surf = type & 0xF, kind/slope/attr[wheel]
 *   from type/cnt; surface 3/4/7 with out.y < 0.25 and the car slot not flagged (857/856) and
 *   s1732 == -1: 4 -> effect_cleanup(id, id, -1) + b1600 = 1, 7 -> b1741 = 1 + func_800C54F0,
 *   3 -> same + func_800B61A8(21, 0, 1, 2) (the locked SOUND wrapper, inlined).
 *   Miss: *surf = kind = slope = attr = 0, out = pos + (0, 200, 0), mat = identity (D_8011418C).
 * Levers found: u8 n / u32 i / list[i] / n = *p++ (as camera_trigger_check); calling
 *   func_800B61A8 instead of open-coding `if (D_8010FFC0) entity_flags_apply(...)` (it also
 *   stopped -5.0f being hoisted into $f26 -- the constant web's savings 20 vs cost 19.75 were
 *   marginal); `if (off != 0) { ... if (bestAbs != 250.0f) { ... return; } }` falling into miss:
 *   (off == 0 and "no hit" share retail's recompute block); `attr &= 0x7FF` (stored value forwarded).
 * Residual: one more uopt temp word than retail at the bottom of the frame (named locals and
 *   the inline-reservation area line up: best 164, poly 168, list 180, quad 278).  Tried: x/z
 *   named or not, unused pads, n = *p; p+1, attr spelling, s16 idx, -200 spelling, an extra
 *   inlined helper (adds 8 bytes per call site).  Next: find which expression owns the extra
 *   temp (ugen spill-slot numbering), e.g. the D_80152818[car->id] address or the inlined
 *   effect_cleanup arguments.
 */
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
