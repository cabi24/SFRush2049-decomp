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

/*
 * camera_trigger_check (0x800C4200, 308 words) -- w10c, frontier wave 10.
 * Historical label; it is a "find the road surface under a point" query (N64-only, no arcade
 * ancestor found):
 *   floor(pos.x), floor(pos.z) -> quadtree cell (handbrake_apply on D_80124EEC, quadrant in `quad`);
 *   the cell's packed polygon list (D_80152460 + child offset, bit 0x10000 from the node mask) is
 *   expanded by func_800ADCE0 into list[]; every polygon that is not type 5/6/15 is tested with
 *   func_800C3AD0 (point-above-polygon, zmin -20, bound = best so far); the polygon with the
 *   smallest |height| wins (out = hit point, mat = its basis via math_utility).
 *   Winner flags 0x2000 -> steering_sensitivity (rounded edge; fails below -5),
 *   0x1000 -> traction_control + func_8008E0B8 on the three basis rows.  out = pos - up * h.
 *   On a miss the probe is nudged (+x, then -x+z, then -2z) and retried; returns the polygon or 0.
 * Shaping (each measured in the whole-program unit):
 *   - u8 n (declared right after quad, it fills the s16 pad) and u32 i with `i < n`: n lives in
 *     s1 across func_800ADCE0, the entry test is beqz, and n is spilled to a temp at loop entry;
 *     with s32/u32 n it is spilled before the call (one extra word);
 *   - `n = *p++; func_800ADCE0(p, n, ...)` keeps p in a0;
 *   - poly = &D_801497F8[list[i]] (no pointer local): the strength-reduced pointer takes s1;
 *   - bestH = 250 (int literal) and bestAbs = 250.0f are separate constant webs (two mtc1);
 *   - fabsf(h) written twice (no named local) gives abs in $f2 and keeps h in $f4;
 *   - best/bestIdx/bestAbs/bestH assigned before the out[] copies;
 *   - mat[1][k] * -out[1] operand order; out[k] = pt[k] + pos[k];
 *   - func_800C3AD0's real parameter order is (poly, wp, q, pt, bound, outIdx, mat, zmin):
 *     callers set up s4, s2, s6, s0, s8, s5, s7, f20 in that order (entity_update agrees).
 */
Poly *camera_trigger_check(f32 *pos, f32 *out, f32 (*mat)[3]) {
    f32 m[3][3];
    u32 i;
    u16 idx;
    u16 bestIdx;
    s16 quad;
    u8 n;
    QNode *node;
    f32 h;
    u32 off;
    u8 *p;
    f32 pt[3];
    u16 list[36];
    f32 bestAbs;
    f32 bestH;
    Poly *poly;
    Poly *best;
    s32 tries;
    f32 x;
    f32 z;

    best = NULL;
    tries = 0;
again:
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
    node = handbrake_apply(D_80124EEC, (s32)x, (s32)z, &quad);
    if (node != NULL) {
        off = node->child[quad];
        if (node->mask & (16 << quad)) {
            off |= 0x10000;
        }
        if (off != 0) {
            p = D_80152460 + off;
            bestH = 250;
            n = *p++;
            bestAbs = 250.0f;
            func_800ADCE0(p, n, list, -1);
            for (i = 0; i < n; i++) {
                poly = &D_801497F8[list[i]];
                if ((poly->type & 0xF) == 5 || (poly->type & 0xF) == 6 || (poly->type & 0xF) == 15) {
                    continue;
                }
                h = bestAbs;
                if (func_800C3AD0(poly, pos, NULL, pt, &h, (s16 *)&idx, (f32 *)m, -20.0f) != 0) {
                    if (fabsf(h) < bestAbs) {
                        best = poly;
                        bestIdx = idx;
                        bestAbs = fabsf(h);
                        bestH = h;
                        out[0] = pt[0];
                        out[1] = pt[1];
                        out[2] = pt[2];
                        math_utility(m, mat);
                    }
                }
            }
            if (bestAbs != 250.0f) {
                if (best->type & 0x2000) {
                    steering_sensitivity((s32)best, bestIdx, pos, out, mat, 250.0f);
                    if (out[1] < -5.0f) {
                        goto fail;
                    }
                    pt[0] = mat[1][0] * -out[1];
                    pt[1] = mat[1][1] * -out[1];
                    pt[2] = mat[1][2] * -out[1];
                } else {
                    if (best->type & 0x1000) {
                        traction_control((s32)best, bestIdx, pos, out, mat);
                        func_8008E0B8(mat[0]);
                        func_8008E0B8(mat[1]);
                        func_8008E0B8(mat[2]);
                    }
                    pt[0] = mat[1][0] * -bestH;
                    pt[1] = mat[1][1] * -bestH;
                    pt[2] = mat[1][2] * -bestH;
                }
                out[0] = pos[0] + pt[0];
                out[1] = pos[1] + pt[1];
                out[2] = pos[2] + pt[2];
                return best;
            }
        }
    }
fail:
    if (tries == 0) {
        tries = 1;
        pos[0] += 1.0f;
        goto again;
    }
    if (tries == 1) {
        tries = 2;
        pos[0] -= 1.0f;
        pos[2] += 1.0f;
        goto again;
    }
    if (tries == 2) {
        tries = 3;
        pos[2] -= 2.0f;
        goto again;
    }
    return NULL;
}
