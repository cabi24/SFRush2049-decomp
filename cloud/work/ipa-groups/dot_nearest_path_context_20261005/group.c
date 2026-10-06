/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Genuine path-following context for func_800E4300. Only that helper is claimed.
 * MP_IntervalPos derives from rushtherock maxpath.c; outer caller and steering
 * context retain their complete native-led reconstructions, explicitly nonmatching.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct {
    u8 pad0[0x220];
    f32 vx; /* 0x220 */
    f32 vy; /* 0x224 */
    f32 vz; /* 0x228 */
    f32 x22C; /* 0x22C */
    f32 y230; /* 0x230 */
    f32 z234; /* 0x234 */
    u8 pad238[0x1B8];
    f32 speed; /* 0x3F0 */
    u8 pad3F4[0x94];
    s8 b488; /* 0x488 */
    u8 pad489[0x5B];
    s8 b4E4; /* 0x4E4 */
    u8 pad4E5[0x14F];
    f32 f634; /* 0x634 */
    f32 f638; /* 0x638 */
    u8 pad63C[0x4];
    s8 b640; /* 0x640 */
    u8 pad641[0x83];
    s16 s6C4; /* 0x6C4 */
    u8 pad6C6[0x6];
    s8 b6CC; /* 0x6CC */
    u8 pad6CD[0x43];
    s32 lastTick; /* 0x710 */
    f32 tickTime; /* 0x714 */
    f32 dt; /* 0x718 */
    u8 pad71C[0x4];
    f32 f720; /* 0x720 */
    u8 pad724[0x4];
    f32 f728; /* 0x728 */
    f32 f72C; /* 0x72C */
    u8 pad730[0x2];
    s8 b732; /* 0x732 */
    u8 pad733[0x55];
    f32 vel[3]; /* 0x788 */
    f32 pos[3]; /* 0x794 */
    u8 pad7A0[0x26];
    s16 unk7C6; /* 0x7C6 */
    u8 pad7C8[0x2];
    s16 s7CA; /* 0x7CA */
    s8 b7CC; /* 0x7CC */
    u8 pad7CD[0x11];
    s8 b7DE; /* 0x7DE */
    u8 pad7DF[0x1];
    s16 s7E0; /* 0x7E0 */
    s16 unk7E2; /* 0x7E2 */
    u8 pad7E4[0x8];
    f32 f7EC; /* 0x7EC */
    u8 pad7F0[0x18];
} D_8014A250_Record;
typedef struct Nav {
    f32 a;                  /* 0x00 */
    f32 b;                  /* 0x04 */
    f32 c;                  /* 0x08 */
    f32 d;                  /* 0x0C */
    f32 spd;                /* 0x10 */
    f32 tgt[3];             /* 0x14 */
    f32 tm;                 /* 0x20 */
    s16 pt;                 /* 0x24 */
    s16 flag;               /* 0x26 */
    s16 sel;                /* 0x28 */
    s16 tk;                 /* 0x2A */
} Nav;
typedef struct {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ s16 last;       /* valid in element 0 */
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s16 count;      /* valid in element 0 */
    /* 0x0A */ u8 padA[0x30 - 0xA];
    /* 0x30 */ s16 start[16];
} Section;                     /* 0x50 */
typedef struct { /* 0x0 */ s16 x, y, z; /* 0x6 */ u8 flag; /* 0x7 */ u8 pad7; } TrackPt;
typedef struct { /* 0x0 */ u16 numPoints; /* 0x2 */ u16 pad2; /* 0x4 */ TrackPt *points; } Track;

typedef struct GameCar {
    u8 prefix[0x314];
    Nav nav;
    u8 suffix[0x3B8 - 0x340];
} GameCar;
extern GameCar player_array[];
extern D_8014A250_Record D_8014A250[];
extern Section D_80151CE8[];
extern Track D_8012E5E8[];
extern s16 D_80152768, D_80154182, D_801527D8[];
extern s8 D_80152744;
extern s8 D_801543C8;
extern s32 D_80120E50;
extern f32 D_801161D4[], D_801161E4[];
extern f32 D_80153F28[], D_80153F48[], D_80153F68[];
extern f32 D_80153F88[][3], D_80154138[][3];
extern f32 D_80154190, D_801543CC;
extern f32 D_80124408;
extern f32 D_8012440C;
extern f32 D_80124410;
extern f32 D_80124414;
extern f32 D_80124418;
extern f32 D_8012441C;
extern f32 D_80124420;
extern f32 D_80124424;
extern f32 D_80124428;
extern f32 D_8012442C;
extern f32 D_80124430;
extern f32 D_80124434;
extern f32 D_80124438;
extern f32 D_8012443C;
extern f32 D_80124454;
extern f32 D_80124458;
extern f32 D_8012445C;
extern f32 D_80124460;
extern f32 D_80124464;
extern f32 D_80124468;
extern f32 D_8012446C;
extern f32 D_80124470;
extern f32 D_80124474;
extern f32 D_80124478;

extern f32 sqrtf(f32), fabsf(f32);
#pragma intrinsic(sqrtf, fabsf)
extern f32 func_8008B3C8(f32 *);
extern f32 func_8008C768(f32, f32);
extern void func_800A61B0(void *, void *, void *);
extern s16 func_800B9338(s16, s16), func_800DC080(s16, s16);
extern f32 func_800BDD90(f32 *);
extern void camera_blend_between(void *, f32 *);
extern void dispatch_handler(s32);
extern void state_utility(s16, s16, void *);
void func_800E451C(D_8014A250_Record *, Nav *, s32);
void func_800E398C(s16);

s16 func_800E4300(s16 *pos, s16 prev, s16 ptIdx, s16 row, s16 cur) {
    Section *ent;
    s16 d;
    s16 diff;
    s16 win;
    s16 best;
    s32 idx;
    s32 k;
    f32 bestd;
    f32 dx, dy, dz, dist;
    TrackPt *pt;

    ent = &D_80151CE8[row];
    d = ent->start[cur] - ent->start[prev];
    if (row + 1 == D_80151CE8[0].count) {
        diff = (D_8012E5E8[cur].numPoints - D_8012E5E8[prev].numPoints) - d;
    } else {
        diff = (ent[1].start[cur] - ent[1].start[prev]) - d;
    }
    if (diff < 0) {
        diff = -diff;
    }
    bestd = D_8012443C;
    best = 0;
    if (diff < 5) {
        win = 5;
    } else {
        win = diff;
    }
    idx = ptIdx + d - win;
    while (idx < 0) {
        idx += D_8012E5E8[cur].numPoints;
    }
    while (idx >= D_8012E5E8[cur].numPoints) {
        idx -= D_8012E5E8[cur].numPoints;
    }
    win = win * 2 + 1;
    k = 0; if (win > 0) for (;;) {
        pt = &D_8012E5E8[cur].points[idx];
        dx = (f32) (pt->x - pos[0]);
        dy = (f32) (pt->y - pos[1]);
        dz = (f32) (pt->z - pos[2]);
        dist = dx * dx + dy * dy + dz * dz;
        if (dist < bestd) {
            bestd = dist;
            best = idx;
        }
        if (idx == D_8012E5E8[cur].numPoints - 1) {
            idx = D_80151CE8[D_80151CE8[0].last].start[cur];
        } else {
            idx++;
        }
        if (++k == win) break;
    }
    return best;
}

static void mp_interval_pos(D_8014A250_Record *m, Nav *cp) {
    TrackPt *mp;
    TrackPt *nmp;
    s16 nmpi;
    f32 x;
    f32 y;
    f32 n[2];

    mp = &D_8012E5E8[cp->sel].points[cp->pt];
    nmpi = func_800B9338(cp->pt, cp->sel);
    nmp = &D_8012E5E8[cp->sel].points[nmpi];
    n[0] = nmp->x - mp->x;
    n[1] = nmp->z - mp->z;
    cp->d = func_800BDD90(n);
    x = m->x22C - mp->x;
    y = m->z234 - mp->z;
    cp->a = (y * n[1]) + (n[0] * x);
    cp->b = (x * n[1]) - (n[0] * y);
    nmpi = func_800B9338(nmpi, cp->sel);
    nmp = &D_8012E5E8[cp->sel].points[nmpi];
    cp->c = ((nmp->x - mp->x) * n[1]) - (n[0] * (nmp->z - mp->z));
}

void func_800E451C(D_8014A250_Record *m, Nav *cp, s32 change_flag) {
    s16 j, high_nextmp, nnmpi, index, node, high_index;
    s32 nmpi;
    f32 x, y, z, score, high_score, delta_spd, mspd;
    f32 xpos, ypos, x1, y1, x2, y2, dx, dy, a1, b1, c1, dist;
    f32 speed, xy_speed, vec1, vec0, dp;
    f32 cur_time;
    s32 i;
    s16 pp[3];

    node = m->unk7C6;
    if (cp->flag >= 0) {
        cp->tm = D_801543CC;
        cp->flag = -1;
    }
    cur_time = D_801543CC;
    if (change_flag && ((cur_time - D_80154190) > 0.5f) &&
        ((D_80154182 == -1) || (D_801527D8[D_80154182] == node))) {
        for (j = 0; j < D_80152768; j++) {
            if (++D_80154182 >= D_80152768) {
                D_80154182 = 0;
            }
            if (D_8014A250[D_801527D8[D_80154182]].s7CA) {
                break;
            }
        }
        D_80154190 = cur_time;

        xpos = m->pos[0];
        ypos = m->pos[2];

        high_score = 0.0f;
        high_index = -1;
        high_nextmp = 0;

        x = m->vx;
        y = m->vz;
        z = m->vy;
        xy_speed = (x * x) + (y * y);
        speed = (z * z) + xy_speed;

        if (xy_speed < 2500.0f) {
            xy_speed = 0.0f;
            vec0 = 0.0f;
            vec1 = 0.0f;
        } else {
            xy_speed = 1.0f / sqrtf(xy_speed);
            vec1 = y * xy_speed;
            vec0 = x * xy_speed;
        }

        for (i = 0; i < 4; i++) {
            score = 0.0f;
            index = i;

            pp[0] = m->pos[0];
            pp[1] = m->pos[1];
            pp[2] = m->pos[2];
            nmpi = func_800E4300(pp, cp->sel, cp->pt, m->unk7E2, index);

            y1 = D_8012E5E8[i].points[nmpi].z;
            x1 = D_8012E5E8[i].points[nmpi].x;

            nnmpi = func_800B9338(nmpi, index);

            y2 = D_8012E5E8[i].points[nnmpi].z;
            x2 = D_8012E5E8[i].points[nnmpi].x;

            dy = y1 - ypos;
            dx = x1 - xpos;

            if (((dy * dy) + (dx * dx)) < 400.0f) {
                dx = x1 - x2;
                dy = y1 - y2;

                a1 = dx;
                b1 = -dy;
                c1 = (x1 * dy) - (y1 * dx);

                dist = sqrtf((a1 * a1) + (b1 * b1));

                if (dist > 0.01f) {
                    dist = fabsf((a1 * ypos) + (b1 * xpos) + c1) / dist;

                    if (dist < 10.0f) {
                        score = 1.0f - (dist * 0.1f);
                    }
                }
            }

            mspd = D_8012E5E8[i].points[nmpi].flag * 1.4666667f;
            delta_spd = fabsf(speed - (mspd * mspd));

            if (delta_spd < (50.0f * (5280.0f / 3600.0f))) {
                score += (1 - (delta_spd / (50.0f * (5280.0f / 3600.0f)))) * 0.5f;
            }

            if (i == cp->tk) {
                score += 1.0f;
            }

            if (xy_speed == 0) {
                score += 1.0f;
            } else {
                dy = y2 - y1;
                dx = x2 - x1;

                dist = 1.0f / sqrtf((dy * dy) + (dx * dx));
                dy *= dist;
                dx *= dist;

                dp = ((vec1 * dy) + (vec0 * dx) + 1) * 0.5f;

                score += dp;
            }

            if (score > high_score) {
                high_score = score;
                high_nextmp = nmpi;
                high_index = i;
            }
        }

        if ((high_index != -1) && (high_index != cp->sel)) {
            cp->pt = high_nextmp;
            cp->tm = cur_time;
            cp->flag = -1;
            cp->sel = high_index;
        }
    }

    mp_interval_pos(m, cp);
}

void func_800E398C(s16 n) {
    D_8014A250_Record *rec;
    D_8014A250_Record *oc;
    Nav *nav;
    f32 tp[3];
    f32 rel[3];
    f32 l[3];
    f32 sv[3];
    f32 sw[3];
    f32 p[3];
    f32 sc;
    s16 flag;
    s16 i;
    s16 k;
    f32 *a0;
    f32 *v0;
    f32 f, g, x, z, d, sp;
    s8 t;

    rec = &D_8014A250[n];
    nav = &player_array[n].nav;
    tp[0] = nav->tgt[0];
    tp[1] = nav->tgt[1];
    tp[2] = nav->tgt[2];
    rel[0] = tp[0] - rec->pos[0];
    rel[1] = tp[1] - rec->pos[1];
    rel[2] = tp[2] - rec->pos[2];
    func_800A61B0(rel, l, (u8 *) rec + 0x7A0);
    i = 0;
    if ((rec->f638 == 0.0f) || (rec->speed < D_80124408)) {
        rec->b7DE = 0;
        l[0] = 0.0f;
        nav->spd = 100.0f;
        nav->tgt[0] = l[0];
        nav->tgt[1] = l[1];
        nav->tgt[2] = l[2];
        do {
            D_80153F88[n][i] = 0.0f;
            i++;
        } while (i < 3);
        return;
    }
    f = (f32) (u32) D_8012E5E8[nav->sel].points[nav->pt].flag * D_8012440C;
    if (rec->speed < f * 0.5f) {
        l[0] = l[0] * D_80124410;
        nav->spd = ((f32) (u32) D_8012E5E8[nav->sel].points[nav->pt].flag * D_8012440C) * 1.5f;
        nav->tgt[0] = l[0];
        nav->tgt[1] = l[1];
        nav->tgt[2] = l[2];
        i = 0;
        do {
            D_80153F88[n][i] = 0.0f;
            i++;
        } while (i < 3);
        rec->b7DE = 0;
        return;
    }
    if (l[2] < fabsf(l[0] + l[0])) {
        nav->tgt[0] = l[0];
        nav->tgt[1] = l[1];
        nav->tgt[2] = l[2];
        i = 0;
        do {
            D_80153F88[n][i] = 0.0f;
            i++;
        } while (i < 3);
        rec->b7DE = 0;
        return;
    }
    d = func_8008B3C8(l);
    g = rec->speed * rec->f634 / d;
    sv[0] = l[0] * g;
    sw[0] = sv[0];
    sv[1] = l[1] * g;
    sw[1] = sv[1];
    sv[2] = l[2] * g;
    sw[2] = sv[2];
    p[0] = rec->pos[0];
    p[1] = rec->pos[1];
    p[2] = rec->pos[2];
    sc = 1.0f;
    flag = 1;
    oc = rec;
    i = 0;
    if (D_80152744 > 0) {
        do {
            k = D_8014A250[i].unk7C6;
            if (k != n) {
                oc = &D_8014A250[k];
                rel[0] = oc->pos[0] - p[0];
                rel[1] = oc->pos[1] - p[1];
                rel[2] = oc->pos[2] - p[2];
                func_800A61B0(rel, l, (u8 *) rec + 0x7A0);
                if (((l[2] * l[2]) + (l[0] * l[0])) <= D_80124418) {
                    if (oc->b7CC == 2) {
                        if ((fabsf(l[0]) < 60.0f) && (fabsf(l[2]) < 200.0f)) {
                            flag = 0;
                        }
                    }
                    if (!(l[2] < 0.0f)) {
                        if (!(8.0f < fabsf(l[0]))) {
                            if (l[2] < 20.0f) {
                                x = D_8012441C;
                            } else if (l[2] < 100.0f) {
                                x = 1.0f - ((100.0f - l[2]) * D_80124414);
                            } else {
                                x = 1.0f;
                            }
                            if (x < sc) {
                                sc = x;
                            }
                        }
                    }
                }
            }
            i++;
        } while (i < D_80152744);
    }
    camera_blend_between(rec, &sc);
    x = sv[0];
    if ((fabsf(x) * 4.0f) < fabsf(sv[2])) {
        if (x < 0.0f) {
            x += (1.0f - sc) * 8.0f;
        } else {
            x -= (1.0f - sc) * 8.0f;
        }
        sv[0] = x;
    } else if (sc < D_80124420) {
        if (x < 0.0f) {
            x = (1.0f - sc) * 4.0f;
        } else {
            x = -(1.0f - sc) * 4.0f;
        }
        sv[0] = x;
    }
    a0 = D_80153F88[n];
    v0 = D_80154138[n];
    t = ((s8 *) &D_8012E5E8[nav->sel].points[nav->pt])[7];
    if ((a0[0] == 0.0f) && (a0[2] == 0.0f)) {
        sv[0] = 0.0f;
        nav->spd = nav->spd * (1.0f - ((1.0f - sc) * D_80124424));
    } else if ((t == 5) || (rec->s6C4 >= 0)) {
        sv[0] = sv[0] * D_80124428;
    } else if (t == 2) {
        sv[0] = sw[0];
        sv[1] = sw[1];
        sv[2] = sw[2];
        nav->spd = nav->spd * (1.0f - ((1.0f - sc) * D_8012442C));
    } else if (t == 1) {
        if ((a0[0] == 0.0f) && (a0[2] == 0.0f)) {
            sv[0] = sw[0];
            sv[1] = sw[1];
            sv[2] = sw[2];
        } else {
            sv[0] = (sw[0] * D_80124430) + (a0[0] * D_80124434);
        }
        nav->spd = nav->spd * (1.0f - ((1.0f - sc) * D_80124438));
    } else if (t == 3) {
        sv[0] = sw[0] - v0[0];
        sw[0] = v0[0];
        sw[1] = v0[1];
        sw[2] = v0[2];
    } else if (t == 4) {
        sv[0] = sw[0] - v0[0];
    } else {
        sv[0] = (a0[0] * 0.5f) + (sv[0] * 0.5f);
    }
    rec->b7DE = flag;
    nav->tgt[0] = sv[0];
    nav->tgt[1] = sv[1];
    nav->tgt[2] = sv[2];
    a0[1] = sv[1];
    a0[0] = sv[0];
    a0[2] = sv[2];
    v0[1] = sw[1];
    v0[2] = sw[2];
    v0[0] = sw[0];
}

void func_800E4B58(D_8014A250_Record *car) {
    Nav *nav;
    TrackPt *s1;
    TrackPt *s2;
    TrackPt *pts;
    s16 idx;
    s16 sel;
    s16 n;
    s32 far;
    f32 t, f0, f1, ax, gg, rem, seg, vy, dx, dy, dz, sum, diff, w, ang;
    f32 *v0;
    f32 *a0;
    s8 dif;

    nav = &player_array[car->unk7C6].nav;
    func_800E451C(car, nav, 1);
    if (nav->d <= nav->a) {
        nav->pt = func_800B9338(nav->pt, nav->sel);
        nav->tm = D_801543CC;
        func_800E451C(car, nav, 0);
    } else if (nav->a < 0.0f) {
        nav->pt = func_800DC080(nav->pt, nav->sel);
        func_800E451C(car, nav, 0);
    }
    idx = func_800B9338(nav->pt, nav->sel);
    t = nav->a / nav->d;
    if (t < 0.0f) {
        t = 0.0f;
    } else if (1.0f < t) {
        t = 1.0f;
    }
    pts = D_8012E5E8[nav->sel].points;
    f0 = (f32) (u32) pts[nav->pt].flag * D_80124454;
    f1 = (f32) (u32) pts[idx].flag * D_80124454;
    nav->spd = ((f1 - f0) * t) + f0;
    ax = nav->b;
    if (0.0f < ax) {
        ax = ax;
    } else {
        ax = -ax;
    }
    if (ax < D_80124458) {
        gg = 1.0f;
    } else if (80.0f < ax) {
        gg = D_8012445C;
    } else {
        gg = 1.0f - ((ax * ax) * D_80124460);
    }
    nav->spd = nav->spd * gg;
    if (nav->spd < 28.0f) {
        nav->spd = 28.0f;
    }
    sel = nav->sel;
    s2 = &D_8012E5E8[sel].points[nav->pt];
    idx = func_800B9338(nav->pt, sel);
    s1 = &D_8012E5E8[nav->sel].points[idx];
    rem = ((nav->a + 80.0f) - nav->d);
    seg = nav->d;
    while (0.0f < rem) {
        s2 = s1;
        idx = func_800B9338(idx, nav->sel);
        s1 = &D_8012E5E8[nav->sel].points[idx];
        dx = (f32) (s1->x - s2->x);
        dy = (f32) (s1->y - s2->y);
        dz = (f32) (s1->z - s2->z);
        seg = sqrtf((dx * dx) + (dy * dy) + (dz * dz));
        rem = rem - seg;
    }
    if (seg < D_80124464) {
        dispatch_handler(5);
        state_utility(80, 160, (void *) &D_80120E50);
        w = 1.0f;
    } else {
        w = (seg + rem) / seg;
    }
    if (w < 0.0f) {
        w = 0.0f;
    } else if (1.0f < w) {
        w = 1.0f;
    }
    nav->tgt[0] = ((f32) (s1->x - s2->x) * w) + (f32) s2->x;
    nav->tgt[1] = ((f32) (s1->y - s2->y) * w) + (f32) s2->y;
    nav->tgt[2] = ((f32) (s1->z - s2->z) * w) + (f32) s2->z;
    n = car->unk7C6;
    func_800E398C(n);
    vy = car->vy;
    seg = nav->spd;
    dif = D_801543C8;
    if (vy < 0.0f) {
        seg = seg * D_801161D4[dif];
    } else {
        f0 = D_801161E4[dif];
        if (seg < f0) {
            seg = f0;
        }
    }
    seg = seg * car->f7EC;
    n = car->unk7C6;
    if ((dif < 2) && ((car->b488 == 40) || (car->b4E4 == 40))) {
        v0 = D_80153F68 + n;
        a0 = D_80153F48 + n;
        *v0 = 0.0f;
        f0 = *v0;
        *a0 = 0.0f;
    } else {
        sum = sqrtf((car->vx * car->vx) + (vy * vy) + (car->vz * car->vz));
        f1 = seg * D_80124468;
        diff = f1 - sum;
        w = diff;
        if (f1 < sum) {
            w = diff * 0.5f;
        }
        v0 = D_80153F68 + n;
        *v0 = *v0 + (w * D_8012446C);
        f0 = *v0;
        if (f0 < -0.75f) {
            a0 = D_80153F48 + n;
            *a0 = *a0 - ((f0 + 0.75f) / 4.0f);
        } else {
            a0 = D_80153F48 + n;
            *a0 = 0.0f;
        }
    }
    if (f0 < 0.0f) {
        *v0 = 0.0f;
        f0 = *v0;
    } else if (1.0f < f0) {
        *v0 = 1.0f;
        f0 = *v0;
    }
    f1 = *a0;
    if (f1 < 0.0f) {
        *a0 = 0.0f;
    } else if (1.0f < f1) {
        *a0 = 1.0f;
    }
    car->f72C = f0;
    car->f728 = *a0;
    nav->tgt[0] = nav->tgt[0] * (car->f634 * D_80124470);
    ang = func_8008C768(nav->tgt[2], nav->tgt[0]);
    if (ang < 0.0f) {
        f1 = 0.0f;
    } else {
        f1 = D_80124474;
        if (!(f1 < ang)) {
            f1 = ang;
        }
    }
    f0 = 1.0f - (f1 * D_80124478);
    D_80153F28[n] = f0;
    car->f720 = f0;
    far = 0;
    if (1.0f < (D_801543CC - nav->tm)) {
        far = 1;
    }
    if (far) {
        car->s7E0 = nav->pt > 10;
    } else {
        car->s7E0 = 0;
    }
    if ((100.0f < fabsf(nav->a)) || (100.0f < fabsf(nav->b))) {
        car->s7E0 = 1;
    }
}
