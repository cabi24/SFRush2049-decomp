/*
 * Per-car rubber-banding ("render_large_objects" at 0x800F93A0) and its two
 * IPA callees. Hand-written from the assembly.
 */
float fabsf(float);
#pragma intrinsic (fabsf)

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;

typedef struct {
    u8 pad0[7];
    u8 flag;
} Slot;

typedef struct {
    /* 0x000 */ u8 pad0[0x400];
    /* 0x400 */ f32 scale;
    /* 0x404 */ u8 pad404[0x7C6 - 0x404];
    /* 0x7C6 */ s16 order;
    /* 0x7C8 */ s16 active;
    /* 0x7CA */ s16 pad7CA;
    /* 0x7CC */ s8 kind;
    /* 0x7CD */ u8 pad7CD[0x7E6 - 0x7CD];
    /* 0x7E6 */ s16 rank;
    /* 0x7E8 */ u8 pad7E8[4];
    /* 0x7EC */ f32 outA;
    /* 0x7F0 */ f32 outB;
    /* 0x7F4 */ u8 pad7F4[0x808 - 0x7F4];
} Car;

typedef struct {
    /* 0x000 */ u8 pad0[8];
    /* 0x008 */ f32 pos[3];
    /* 0x014 */ u8 pad14[0xEE - 0x14];
    /* 0x0EE */ s8 place;
    /* 0x0EF */ u8 pad0EF[0x100 - 0xEF];
    /* 0x100 */ f32 dist;
    /* 0x104 */ u8 pad104[0x356 - 0x104];
    /* 0x356 */ s16 slot;
    /* 0x358 */ u8 pad358;
    /* 0x359 */ s8 state;
    /* 0x35A */ u8 pad35A[0x3B8 - 0x35A];
} Rec;

extern Car D_8014A250[];
extern Rec player_array[];
extern s16 active_player_count;
extern s8 D_80152030;
extern s8 D_80150F14;
extern Slot D_80153E88[];
extern f32 D_80124310;
extern f32 D_80124314;
extern f32 D_80124318;
extern f32 D_8012431C;
extern f32 D_80124638;

f32 func_800F92C8(f32 a, f32 b, f32 x, f32 outA, f32 outB);
void func_800DE860(void);

f32 func_800F92C8(f32 a, f32 b, f32 x, f32 outA, f32 outB)
{
    f32 d;
    f32 t;

    if (a < b) {
        if (x < a) return outA;
        if (b < x) return outB;
    } else {
        if (x < b) return outB;
        if (a < x) return outA;
    }
    d = a - b;
    if (fabsf(d) < D_80124638) {
        return (outA + outB) * 0.5f;
    }
    t = (outA - outB) / d;
    return t * x + (outA - t * a);
}

void func_800DE860(void)
{
    s16 i;
    s16 best;
    s32 j;
    f32 s;
    f32 d;
    f32 bd;
    f32 k1;
    f32 k2;

    if ((active_player_count == 1 && D_80152030 < 5) || D_80150F14 == 0) {
        for (j = 0; j < 6; j++) {
            if (D_80153E88[j].flag == 0 || D_80153E88[j].flag == 6) {
                D_8014A250[j].scale = 1.0f;
            }
        }
    } else {
        best = -1;
        for (i = 0; i < 6; i++) {
            if (D_8014A250[i].active != 0 && player_array[i].state < 2 &&
                (D_8014A250[i].kind == 2 || D_80152030 == 5)) {
                if (best == -1) {
                    best = i;
                } else if (player_array[best].dist < player_array[i].dist) {
                    best = i;
                }
            }
        }
        k1 = D_80124310;
        k2 = D_80124314;
        bd = player_array[best].dist;
        for (i = 0; i < 6; i++) {
            if (D_8014A250[i].active != 0 && player_array[i].state < 2 &&
                (D_8014A250[i].kind == 2 || D_80152030 == 5)) {
                d = bd - player_array[i].dist;
                if (d > k2) {
                    s = k1 + 1.0f;
                } else {
                    s = d * k1 / k2 + 1.0f;
                }
                if (D_80150F14 == 1) {
                    s = (1.0f - s) * 0.5f + 1.0f;
                }
                D_8014A250[i].scale = D_8014A250[i].scale * D_80124318 + D_8012431C * s;
            }
        }
    }
}

#define KK(a) (D_8012463C[((a) - 0x8012463C) / 4])
#define FA(a, b) (1.0f - ((KK(a) * s + KK(b)) * m))
#define FB(a, b, c) ((KK(a) * s + 1.0f) - (m * (KK(b) * s + KK(c))))
#define FC(a, b, c, d) ((KK(a) * s + KK(b)) - (m * (KK(c) * s + KK(d))))
#define FD(a, b) (KK(a) - ((0.5f * s + KK(b)) * m))

extern f32 D_8012463C[];
extern s8 D_80152744;
extern s32 D_801174B4;
extern f32 D_801543CC;
extern s32 D_8014A110;
extern s8 D_8014978C;
extern s8 D_80152570;
extern s16 D_80154484[][38];
extern s8 D_8013FECB;
extern s8 D_80152718;
extern s8 D_80152015;

void render_large_objects(void)
{
    s16 clist[6];
    s16 nlist[6];
    s16 cnt[6];
    s16 byrank[6];
    s16 place[8];
    f32 diff[3];
    f32 dist2[6][6];
    s8 n;
    s16 i;
    s16 j;
    s16 k;
    s16 c;
    s16 d;
    s16 t1;
    s16 nn;
    s16 nc;
    s16 left;
    s16 right;
    s16 cahead;
    s16 cbehind;
    s16 P;
    s32 lvl;
    s32 v1;
    f32 s;
    f32 m;
    f32 f0;
    f32 f2;
    f32 f12;
    f32 f16;
    f32 f24;
    f32 f26;
    f32 f30;
    f32 a;
    f32 v;
    Car *cp;

    func_800DE860();
    n = D_80152744;
    nn = 0;
    nc = 0;
    for (i = 0; i < n; i++) {
        c = D_8014A250[i].order;
        place[c] = player_array[c].place;
    }
    for (i = 0; i < n; i++) {
        c = 0;
        for (j = 0; j < n; j++) {
            c = D_8014A250[j].order;
            if (i == place[c]) {
                break;
            }
        }
        byrank[i] = c;
        if (D_8014A250[c].kind == 2) {
            clist[nc] = c;
            nc++;
        } else {
            nlist[nn] = c;
            nn++;
        }
    }
    for (k = 0; k < nn; k++) {
        player_array[nlist[k]].slot = k;
    }
    if (D_801174B4 & 8) {
        for (i = 0; i < n; i++) {
            c = D_8014A250[i].order;
            if (D_8014A250[c].kind == 1) {
                D_8014A250[c].outA = 1.0f;
            }
        }
    } else if (nn != 0) {
        if (D_801543CC < 5.0f) {
            for (k = 0; k < nn; k++) {
                c = nlist[k];
                D_8014A250[c].outA = 1.0f;
                D_8014A250[c].outB = (f32) (nn - k - 1) * KK(0x80124640) + KK(0x8012463C);
            }
        } else {
            for (i = 0; i < n; i++) {
                c = D_8014A250[i].order;
                cnt[c] = 0;
                for (j = i; j < n; j++) {
                    d = D_8014A250[j].order;
                    if (c == d) {
                        dist2[c][d] = KK(0x80124644);
                    } else {
                        for (k = 0; k < 3; k++) {
                            diff[k] = player_array[c].pos[k] - player_array[d].pos[k];
                        }
                        f16 = diff[2] * diff[2] + (diff[0] * diff[0] + diff[1] * diff[1]);
                        dist2[d][c] = f16;
                        dist2[c][d] = f16;
                    }
                }
            }
            for (k = 0; k < nn; k++) {
                c = nlist[k];
                D_8014A250[c].rank = c;
            }
            for (i = 0; i < nc; i++) {
                for (j = i + 1; j < nc; j++) {
                    if (dist2[clist[i]][clist[j]] < KK(0x80124648)) {
                        cnt[clist[i]] += 10;
                        cnt[clist[j]] += 10;
                    }
                }
            }
            for (i = 0; i < nc; i++) {
                c = clist[i];
                if (cnt[c] < 10) {
                    P = place[c];
                    left = -1;
                    right = -1;
                    if (P > 0) {
                        d = byrank[P - 1];
                        if (D_8014A250[d].kind == 1 && cnt[d] == 0) {
                            left = d;
                        }
                    }
                    if (P < n - 1) {
                        d = byrank[P + 1];
                        if (D_8014A250[d].kind == 1 && cnt[d] == 0) {
                            right = d;
                        }
                    }
                    if (left != -1 && right != -1) {
                        if (dist2[right][c] < dist2[left][c]) {
                            left = right;
                        }
                    } else if (left == -1) {
                        left = right;
                    }
                    if (left != -1) {
                        cnt[c]++;
                        cnt[left]++;
                        D_8014A250[left].rank = c;
                    }
                }
            }
            for (i = 0; i < n; i++) {
                c = D_8014A250[i].order;
                cp = &D_8014A250[c];
                if (cp->pad7CA != 0) {
                    t1 = cp->rank;
                    P = place[c];
                    lvl = D_8014A110;
                    s = 1.0f - (f32) D_80152030 / 5.0f;
                    if (D_80152030 == 5) {
                        m = 0.0f;
                    } else {
                        f0 = (f32) (D_80152744 - 1);
                        if (P == 0) {
                            m = 0.5f / f0;
                        } else {
                            m = (f32) P / f0;
                        }
                    }
                    if (lvl == 3) {
                        lvl = D_8014978C;
                        v1 = 0;
                        if (D_80152570 != 0) {
                            v1 = 6;
                        }
                        s *= (f32) D_80154484[cp->order][lvl + v1] * 0.125f + 0.75f;
                    }
                    if (t1 != c) {
                        f16 = player_array[c].dist - player_array[t1].dist;
                        if (f16 < -20.0f) {
                            f24 = FA(0x80124660, 0x80124664);
                            f12 = func_800F92C8(-300.0f, -60.0f, f16, FC(0x80124650, 0x80124654, 0x80124658, 0x8012465C), f24);
                            f26 = FB(0x80124668, 0x8012466C, 0x80124670);
                            f2 = f26;
                        } else {
                            if (cnt[t1] < 2) {
                                f26 = FB(0x8012467C, 0x80124680, 0x80124684);
                                f2 = func_800F92C8(200.0f, 60.0f, f16, FD(0x80124678, 0x80124674), f26);
                                f24 = FA(0x80124688, 0x8012468C);
                            } else {
                                f26 = FB(0x80124694, 0x80124698, 0x80124690);
                                f2 = f26;
                                f24 = FA(0x8012469C, 0x80124690);
                            }
                            f12 = f24;
                        }
                    } else if (P == 0) {
                        f16 = player_array[c].dist - player_array[byrank[1]].dist;
                        if (f16 < 200.0f) {
                            f24 = FA(0x801246A0, 0x801246A4);
                            f12 = func_800F92C8(200.0f, 0.0f, f16, f24, FC(0x801246A8, 0x801246AC, 0x801246B0, 0x801246B4));
                            f26 = FB(0x801246B8, 0x801246BC, 0x801246C0);
                            f2 = f26;
                        } else {
                            f26 = FB(0x801246CC, 0x801246D0, 0x801246D4);
                            f2 = func_800F92C8(500.0f, 200.0f, f16, FD(0x801246C8, 0x801246C4), f26);
                            f24 = FA(0x801246D8, 0x801246DC);
                            f12 = f24;
                        }
                    } else if (P + 1 == D_80152744) {
                        f16 = player_array[c].dist - player_array[byrank[D_80152744 - 2]].dist;
                        if (f16 < -150.0f) {
                            f24 = FA(0x801246E0, 0x801246E4);
                            f12 = func_800F92C8(-150.0f, -300.0f, f16, f24, FC(0x801246E8, 0x801246EC, 0x801246F0, 0x801246F4));
                            f26 = FB(0x801246F8, 0x801246FC, 0x80124700);
                            f2 = f26;
                        } else {
                            f26 = FB(0x80124704, 0x80124708, 0x8012470C);
                            f2 = func_800F92C8(-150.0f, 0.0f, f16, f26, FD(0x80124714, 0x80124710));
                            f24 = FA(0x80124718, 0x8012471C);
                            f12 = f24;
                        }
                    } else {
                        f0 = player_array[c].dist;
                        f16 = f0 - player_array[byrank[P - 1]].dist;
                        f30 = f0 - player_array[byrank[P + 1]].dist;
                        cahead = 0;
                        cbehind = 0;
                        if (f16 > -150.0f && f30 < 150.0f) {
                            f0 = (f16 + f30) / 2.0f;
                            if (f0 < 0.0f) {
                                f24 = FA(0x80124720, 0x80124724);
                                f12 = func_800F92C8(f16 - f0, -200.0f, f16, f24, FC(0x80124728, 0x8012472C, 0x80124730, 0x80124734));
                                f26 = FB(0x80124738, 0x8012473C, 0x80124740);
                                f2 = f26;
                            } else {
                                f26 = FB(0x80124744, 0x80124748, 0x8012474C);
                                f2 = func_800F92C8(f30 - f0, 200.0f, f16, f26, FD(0x80124754, 0x80124750));
                                f24 = FA(0x80124758, 0x8012475C);
                                f12 = f24;
                            }
                        } else {
                            for (j = 0; j < P; j++) {
                                if (D_8014A250[byrank[j]].kind == 2) {
                                    cahead++;
                                }
                            }
                            for (j = P; j < D_80152744; j++) {
                                if (D_8014A250[byrank[j]].kind == 2) {
                                    cbehind++;
                                }
                            }
                            if (cahead >= cbehind) {
                                if (f16 < -150.0f) {
                                    f24 = FA(0x80124760, 0x80124764);
                                    f12 = func_800F92C8(-150.0f, -300.0f, f16, f24, FC(0x80124768, 0x8012476C, 0x80124770, 0x80124774));
                                    f26 = FB(0x80124778, 0x8012477C, 0x80124780);
                                    f2 = f26;
                                } else {
                                    f26 = FB(0x80124784, 0x80124788, 0x8012478C);
                                    f2 = func_800F92C8(-150.0f, 0.0f, f16, f26, FD(0x80124794, 0x80124790));
                                    f24 = FA(0x80124798, 0x8012479C);
                                    f12 = f24;
                                }
                            } else if (f30 < 150.0f) {
                                f24 = FA(0x801247A0, 0x801247A4);
                                f12 = func_800F92C8(150.0f, 0.0f, f16, f24, FC(0x801247A8, 0x801247AC, 0x801247B0, 0x801247B4));
                                f26 = FB(0x801247B8, 0x801247BC, 0x801247C0);
                                f2 = f26;
                            } else {
                                f26 = FB(0x801247CC, 0x801247D0, 0x801247D4);
                                f2 = func_800F92C8(500.0f, 150.0f, f16, FD(0x801247C8, 0x801247C4), f26);
                                f24 = FA(0x801247D8, 0x801247DC);
                                f12 = f24;
                            }
                        }
                    }
                    if (D_8013FECB != 0 || D_80152718 != 0) {
                        f12 = f24;
                    }
                    if (D_80152015 != 0 && place[c] < player_array[clist[nc - 1]].place) {
                        f2 = f26;
                        f12 = f24;
                    }
                    a = cp->outA;
                    if (f2 < a) {
                        v = a - KK(0x801247E0);
                        if (v < f2) {
                            v = f2;
                        }
                    } else {
                        v = a + KK(0x801247E4);
                        if (f2 < v) {
                            v = f2;
                        }
                    }
                    cp->outA = v;
                    a = cp->outB;
                    if (f12 < a) {
                        v = a - KK(0x801247E8);
                        if (v < f12) {
                            v = f12;
                        }
                    } else {
                        v = a + KK(0x801247E8);
                        if (f12 < v) {
                            v = f12;
                        }
                    }
                    cp->outB = v;
                }
            }
        }
    }
}
