/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Car body versus surface, historical name camera_play_script, 0x800C5644.
 * MAT3 matched CollideCarBodyAgainstSurface reconstruction supports reusing ka
 * in the corner and endpoint loops. It is not recovered original source text.
 * Source shaping: ex * 1.0f (two occurrences) makes the operand an expression
 * before IDO folds the identity. Removing both leaves one commuted mul.s word.
 * This is NOT a general IEEE-754 identity assertion, especially for NaNs,
 * signaling exceptions, or flush-to-zero modes. The stock target compile is
 * word-identical and emits no extra multiplication. No FP mode is changed.
 * The mixed zero literal spellings and named locals are inherited from w18d.
 */
#define NULL ((void *)0)
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct PV { s16 x, y, z; u16 w; } PV;
extern PV *D_8015201C;
extern s32 D_80152568;
extern s8 D_801174CC[16];
void effect_cleanup(s8, s8, s8);
void func_800A61B0(void *, void *, void *);
s32 func_800AD5D0(u8 *, s32, s16 *);
s32 func_800B61A8(s32, s32, s32, unsigned char);
void func_800C54F0(s16, s32);
void math_utility(void *, void *);
void func_800AD650(void *, void *);
float fabsf(float);
#pragma intrinsic (fabsf)
extern s8 D_801174C4[];
typedef struct Poly {
    u16 type;
    u16 cnt;
    u8 body[0x12];
    u16 off;
} Poly;
typedef struct CCol {
    Poly *poly;
    f32 m[9];
    s16 idx;
    s16 edge;
    f32 k;
} CCol;
typedef struct CCar {
    u8 p0[628];
    f32 B628[4][3];
    f32 C676[4][3];
    u8 p724[24];
    f32 mat[9];
    u8 p784[816];
    s8 b1600;
    u8 p1601[131];
    s16 f6C4;
    u8 p1734[7];
    s8 b1741;
    u8 p1742[248];
    s16 pidx;
} CCar;
typedef struct PCar {
    u8 p0[0x358];
    s8 f358;
    s8 f359;
    u8 p35A[0x3B8 - 0x35A];
} PCar;
extern PCar player_array[8];
typedef struct Link {
    u8 p0[7];
    u8 kind;
} Link;
extern Link D_80153E88[];
void func_800C36A0(CCar *car, CCol *col);
void camera_play_script(CCar *car, Poly *poly, CCol *col);

#define DECODE(v, e) \
    (v)[0] = (f32) (((e)->x << 5) + (((e)->w & 0x7C00) >> 10)) * 0.03125f; \
    (v)[1] = (f32) (((e)->y << 5) + (((e)->w & 0x3E0) >> 5)) * 0.03125f; \
    (v)[2] = (f32) (((e)->z << 5) + ((e)->w & 0x1F)) * 0.03125f

void camera_play_script(CCar *car, Poly *poly, CCol *col) {
    u32 n;
    s32 type;
    u16 idx[16];
    PV *v;
    f32 org[3];
    f32 d[3];
    f32 pts[8][3];
    f32 pv[16][2];
    f32 mtx[9];
    s32 conv;
    s32 ka;
    s32 kb;
    s32 k;
    s32 e;
    f32 best;
    f32 da;
    f32 db;
    f32 dy;
    f32 dx;
    f32 dz;
    f32 t;
    f32 u;
    f32 px;
    s32 bestk;
    s32 beste;
    f32 pz;
    f32 ex;
    f32 hx;
    f32 hz;
    f32 ez;
    s32 i;
    Poly *old;

    conv = 0;
    n = poly->cnt;
    n &= 0xF;
    func_800AD5D0((u8 *) (poly->off + D_80152568), n, (s16 *) idx);
    i = idx[0];
    v = &D_8015201C[i];
    DECODE(org, v);
    func_800AD650(poly->body, mtx);
    for (ka = 0; ka < 4; ka++) {
        d[0] = car->B628[D_801174C4[ka]][0] - org[0];
        d[1] = car->B628[D_801174C4[ka]][1] - org[1];
        d[2] = car->B628[D_801174C4[ka]][2] - org[2];
        func_800A61B0(d, pts[ka], mtx);
    }
    type = poly->type & 0xF;
    if (type == 5 || type == 6) {
        dz = -3.0f;
    } else if (mtx[3] * car->mat[3] + mtx[4] * car->mat[4] + mtx[5] * car->mat[5] < -0.707f) {
        dz = -4.5f;
    } else {
        dz = -0.5f;
    }
    func_800A61B0(&car->mat[3], d, mtx);
    for (ka = 0; ka < 4; ka++) {
        pts[ka + 4][0] = pts[ka][0] + d[0] * dz;
        pts[ka + 4][1] = pts[ka][1] + d[1] * dz;
        pts[ka + 4][2] = pts[ka][2] + d[2] * dz;
    }
    best = 0.0f;
    bestk = -1;
    for (e = 0; e < 8; e++) {
        ka = D_801174CC[e];
        kb = D_801174CC[e + 8];
        da = pts[ka][1];
        db = pts[kb][1];
        if (da * db < 0.0f) {
            if (conv == 0) {
                for (k = n - 1; k > 0; k--) {
                    v = &D_8015201C[idx[k]];
                    pv[k][0] = (f32) ((v->x << 5) + ((v->w & 0x7C00) >> 10)) * 0.03125f;
                    pv[k][1] = (f32) ((v->z << 5) + (v->w & 0x1F)) * 0.03125f;
                }
                conv = 1;
            }
            dx = pts[kb][0] - pts[ka][0];
            dz = pts[kb][2] - pts[ka][2];
            dy = db - da;
            if (dy != 0.0f) {
                u = fabsf(da / dy);
                hx = pts[ka][0] + dx * u;
                hz = pts[ka][2] + dz * u;
                if (dy > 0.0f) {
                    t = u;
                    dx = -dx;
                    dz = -dz;
                } else {
                    t = 1.0f - u;
                }
                px = pv[1][0];
                pz = 0.0f;
                if (!(-px * hz > 0.0f)) {
                    if (dz != 0.f) {
                        u = -hz / dz;
                        if (u > 0.0f && u < t) {
                            t = u;
                        }
                    }
                    for (k = 2; k < n; k++) {
                        ez = pv[k][1] - pz;
                        ex = pv[k][0] - px;
                        u = (hx - px) * ez;
                        if (ex * 1.0f * (hz - pz) < u) {
                            goto next;
                        }
                        u = ex * dz - ez * dx;
                        if (u != 0.0f) {
                            u = ((hx - px) * ez - ex * 1.0f * (hz - pz)) / u;
                            if (u > 0.0f && u < t) {
                                t = u;
                            }
                        }
                        px = pv[k][0];
                        pz = pv[k][1];
                    }
                    if (!(pz * hx < px * hz)) {
                        u = px * dz - pz * dx;
                        type = poly->type & 0xF;
                        if (u != 0.0f) {
                            u = (pz * hx - px * hz) / u;
                            if (u > 0.0f && u < t) {
                                t = u;
                            }
                        }
                        if (type == 4) {
                            if (((PCar *) player_array)[car->pidx].f359 == 0 && ((PCar *) player_array)[car->pidx].f358 == 0 && car->f6C4 == -1) {
                                effect_cleanup(car->pidx, car->pidx, -1);
                                car->b1600 = 1;
                            }
                        } else if (type == 7) {
                            if (((PCar *) player_array)[car->pidx].f359 == 0 && ((PCar *) player_array)[car->pidx].f358 == 0 && car->f6C4 == -1) {
                                car->b1741 = 1;
                                func_800C54F0(car->pidx, 1);
                            }
                        } else if (type == 3) {
                            if (((PCar *) player_array)[car->pidx].f359 == 0 && ((PCar *) player_array)[car->pidx].f358 == 0 && car->f6C4 == -1) {
                                car->b1741 = 1;
                                func_800C54F0(car->pidx, 1);
                                if (D_80153E88[car->pidx].kind == 6) {
                                    func_800B61A8(21, 0, 1, 2);
                                }
                            }
                            goto end;
                        }
                        t *= dy;
                        if (t < 0.f) {
                            t = -t;
                        }
                        if (best < t) {
                            k = (-da / db < 1.0f) ? ka : kb;
                            org[0] = car->B628[D_801174C4[k]][0] - car->C676[D_801174C4[k]][0];
                            org[1] = car->B628[D_801174C4[k]][1] - car->C676[D_801174C4[k]][1];
                            org[2] = car->B628[D_801174C4[k]][2] - car->C676[D_801174C4[k]][2];
                            func_800A61B0(org, d, mtx);
                            if (!(d[1] >= 0.0f)) {
                                best = t;
                                beste = e;
                                bestk = k;
                            }
                        }
                    }
                }
            }
        }
next:;
    }
    if (bestk >= 0) {
        if (col->poly != NULL) {
            if (col->k < best) {
                func_800C36A0(car, col);
                col->poly = NULL;
                camera_play_script(car, poly, col);
                return;
            } else {
                old = col->poly;
                col->poly = poly;
                math_utility(mtx, col->m);
                col->idx = bestk;
                col->edge = beste;
                col->k = best;
                func_800C36A0(car, col);
                col->poly = NULL;
                camera_play_script(car, old, col);
                return;
            }
        } else {
            col->poly = poly;
            math_utility(mtx, col->m);
            col->idx = bestk;
            col->edge = beste;
            col->k = best;
        }
    }
end:;
}
