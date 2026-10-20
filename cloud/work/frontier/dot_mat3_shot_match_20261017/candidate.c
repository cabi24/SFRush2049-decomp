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
s16 input_process_controller(Poly *poly, f32 *p1, f32 *p2, f32 *out, f32 *vcOut, f32 *mat, s32 flag, s16 *outIdx, f32 rad2);
#define FLOORF(v) (((v) < 0.0f && (f32)(s32)(v) != (v)) ? (v) - 1.0f : (v))

/* ShotToWorldCollision, 0x800ADD58, 3580 bytes.
 * Reconstructed from the historical N64 candidate with MAT3 donor guidance.
 * The collision result has the callee's signed-short type. Quadtree z floors
 * are evaluated in their call arguments. Traversal interpolation is written
 * as current position plus scaled displacement. No forced compiler events.
 */
Poly *input_deadzone_apply(f32 *start, f32 *end, f32 (*mat)[3], f32 radius, s32 snap, s32 skipType) {
    f32 m[3][3];
    u32 i;
    s16 quad;
    s16 endQuad;
    f32 t;
    s16 mx;
    s16 mz;
    u8 n;
    f32 cur[3];
    f32 d[3];
    f32 np[3];
    f32 vc[3];
    s16 state;
    u16 idx;
    u16 bestIdx;
    u16 list[35];
    u32 off;
    s16 minx;
    s16 maxx;
    s16 minz;
    s16 maxz;
    f32 x;
    f32 z;
    QNode *node;
    QNode *endNode;
    Poly *poly;
    Poly *best;
    u8 *p;

    best = NULL;
    cur[0] = start[0];
    cur[1] = start[1];
    cur[2] = start[2];
    x = FLOORF(cur[0]);
    node = handbrake_apply(D_80124EEC, (s32)x, (s32)(z = FLOORF(cur[2])), &quad);
    if (node == NULL) {
        return NULL;
    }
    if (end[0] < 0.0f && (f32)(s32)end[0] != end[0]) {
        x = end[0] - 1.0f;
    } else {
        x = end[0];
    }
    endNode = handbrake_apply(D_80124EEC, (s32)x, (s32)(z = FLOORF(end[2])), &endQuad);
    for (;;) {
        mx = node->x0 + node->x1;
        if (mx < 0) {
            mx++;
        }
        mz = node->y0 + node->y1;
        if (mz < 0) {
            mz++;
        }
        mx >>= 1;
        mz >>= 1;
        minx = (quad & 1) ? mx : node->x0;
        maxx = (quad & 1) ? node->x1 : mx;
        minz = (quad & 2) ? node->y0 : mz;
        maxz = (quad & 2) ? mz : node->y1;
        off = node->child[quad];
        if (node->mask & (16 << quad)) {
            off |= 0x10000;
        }
        if (off != 0) {
            p = D_80152460 + off;
            n = *p++;
            func_800ADCE0(p, n, list, -1);
            for (i = 0; i < n; i++) {
                poly = &D_801497F8[list[i]];
                if ((poly->type & 0xF) == 15 || (poly->type & 0xF) == skipType) {
                    continue;
                }
                state = input_process_controller(poly, cur, end, np, vc, (f32 *)m, 0, (s16 *)&idx, radius * radius);
                if (state != 0) {
                    if (np[0] < minx || maxx <= np[0] || np[2] < minz || maxz <= np[2]) {
                        continue;
                    }
                    end[0] = np[0];
                    end[1] = np[1];
                    end[2] = np[2];
                    math_utility(m, mat);
                    best = poly;
                    bestIdx = idx;
                }
            }
        }
        if (best != NULL) {
            if (snap != 0) {
                if (best->type & 0x2000) {
                    steering_sensitivity((s32)best, bestIdx, end, vc, mat, 250.0f);
                } else if (best->type & 0x1000) {
                    traction_control((s32)best, bestIdx, end, vc, mat);
                    func_8008E0B8(mat[0]);
                    func_8008E0B8(mat[1]);
                    func_8008E0B8(mat[2]);
                }
            }
            return best;
        }
        if (node == endNode && quad == endQuad) {
            return NULL;
        }
        d[0] = end[0] - cur[0];
        d[1] = end[1] - cur[1];
        d[2] = end[2] - cur[2];
        if (d[0] < 0.0f) {
            np[0] = minx;
            t = (np[0] - cur[0]) / d[0];
            np[1] = cur[1] + d[1] * t;
            np[2] = cur[2] + d[2] * t;
            if (np[2] == minz && d[2] < 0.0f) {
                node = handbrake_apply(D_80124EEC, minx - 1, minz - 1, &quad);
                goto moved;
            }
            if (np[2] == maxz && d[2] < 0.0f) {
                node = handbrake_apply(D_80124EEC, minx - 1, maxz - 1, &quad);
                goto moved;
            }
            if (minz <= np[2] && np[2] <= maxz) {
                node = handbrake_apply(D_80124EEC, minx - 1, (s32)FLOORF(np[2]), &quad);
                goto moved;
            }
        } else if (d[0] > 0.0f) {
            np[0] = maxx;
            t = (np[0] - cur[0]) / d[0];
            np[1] = cur[1] + d[1] * t;
            np[2] = cur[2] + d[2] * t;
            if (np[2] == minz && d[2] < 0.0f) {
                node = handbrake_apply(D_80124EEC, maxx, minz - 1, &quad);
                goto moved;
            }
            if (np[2] == maxz && d[2] < 0.0f) {
                node = handbrake_apply(D_80124EEC, maxx, maxz - 1, &quad);
                goto moved;
            }
            if (minz <= np[2] && np[2] <= maxz) {
                node = handbrake_apply(D_80124EEC, maxx, (s32)FLOORF(np[2]), &quad);
                goto moved;
            }
        }
        if (d[2] < 0.0f) {
            np[2] = minz;
            t = (np[2] - cur[2]) / d[2];
            np[1] = cur[1] + d[1] * t;
            np[0] = cur[0] + d[0] * t;
            if (np[0] == minx && d[0] < 0.0f) {
                node = handbrake_apply(D_80124EEC, minx - 1, minz - 1, &quad);
                goto moved;
            }
            if (np[0] == maxx && d[0] < 0.0f) {
                node = handbrake_apply(D_80124EEC, maxx - 1, minz - 1, &quad);
                goto moved;
            }
            if (minx <= np[0] && np[0] <= maxx) {
                node = handbrake_apply(D_80124EEC, (s32)FLOORF(np[0]), minz - 1, &quad);
                goto moved;
            }
        } else if (d[2] > 0.0f) {
            np[2] = maxz;
            t = (np[2] - cur[2]) / d[2];
            np[1] = cur[1] + d[1] * t;
            np[0] = cur[0] + d[0] * t;
            if (np[0] == minx && d[0] < 0.0f) {
                node = handbrake_apply(D_80124EEC, minx - 1, maxz, &quad);
                goto moved;
            }
            if (np[0] == maxx && d[0] < 0.0f) {
                node = handbrake_apply(D_80124EEC, maxx - 1, maxz, &quad);
                goto moved;
            }
            if (minx <= np[0] && np[0] <= maxx) {
                node = handbrake_apply(D_80124EEC, (s32)FLOORF(np[0]), maxz, &quad);
                goto moved;
            }
        }
moved:
        cur[0] = np[0];
        cur[1] = np[1];
        cur[2] = np[2];
        if (node == NULL) {
            return NULL;
        }
    }
}
