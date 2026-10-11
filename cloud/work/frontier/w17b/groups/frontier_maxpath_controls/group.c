/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* frontier_maxpath_controls: the N64 drone path-following code, a port of the arcade
 * game/maxpath.c rev 3.48 (historicalsource/rushtherock 845329d7, (c) 1996 Atari Corporation).
 *
 *   func_800E4B58  MaxPathControls (kept root).  Inlines MP_FindInterval, MP_TargetSpeed,
 *                  MP_TargetSteerPos, AdjustSpeed, AdjustSteer; calls avoid_areas.
 *   func_800E398C  avoid_areas(drone_index): IPA register parameter in s7, clobbers s0-s8 and
 *                  $f20-$f30 unsaved (MaxPathControls saves them).
 *   func_800E451C  MP_IntervalPos (m in s5, cp in s6), inlines mp_interval_pos.
 *   func_800E4300  nearest track point search used by MP_IntervalPos.
 *   Inlined helpers, named after the locked 2-word stubs they leave (unit order proves the
 *   mapping: the stubs land at E397C, E3984 | E42E8, E42F0, E42F8 | E4B50 exactly as in retail):
 *     func_800E397C AdjustSteer, func_800E3984 AdjustSpeed, func_800E42E8 MP_TargetSteerPos,
 *     func_800E42F0 MP_TargetSpeed, func_800E42F8 mp_interval_pos, func_800E4B50 MP_FindInterval.
 *
 * Disclosures (no stand-ins, no implicit int, no pad arrays, no compiled-out reads):
 * - Unused locals, all genuine arcade declarations, each the exact frame residual:
 *     AdjustSpeed `i, slip, spddir, cur_time` (4 slots): MaxPathControls frame 304 (retail),
 *       288 without them; with them every home matches (cp 296, tgtdist 200, 144/120/116).
 *     MP_TargetSpeed `TrackPt *mp, *nmp` (2 slots, unused: retail keeps both addresses in ring
 *       temps, so the expressions index D_8012E5E8 directly): frame 296 without them.
 *     avoid_areas `tmp2, temp[3], dir_list[6][3], dir_weight[6], cur_rate[3]` (31 slots):
 *       frame 288 (retail), 160 without them; rpos/pos/tpos/own_rwr/dir/save_dir homes match.
 *     mp_interval_pos: `invdist` is computed and unused, as in the earlier packet (arcade source).
 * - Float literal spellings are chosen per site: uopt keys constants by their spelling (`0`,
 *   `0.0f`, `0.f`, `1`, `1.0f` are different constants), which decides which uses share a
 *   register web, which are hoisted, and which are rematerialised. Each literal is the value
 *   the arcade source uses; only the spelling was selected (traced, see RESULTS.md).
 * - Integer literals kept from the arcade where the float form would change code:
 *   AdjustSpeed `/ 4` (not strength-reduced, unlike `/ 4.0f`), MP_TargetSteerPos `interp = 1`.
 * - veccopy/vecsub/scalmul are one-line macros (the arcade function names): as1 orders the
 *   three stores by source line, which gives retail's [1],[0],[2] store orders.
 * - avoid_areas reassigns `m` in the traffic loop as the arcade does; the N64-only
 *   `m->s6C4 >= 0` hint test therefore reads the last scanned car (retail reloads 60(sp)).
 * - MP_IntervalPos: D_80152768 volatile s16 as in src/blob/func_800EC914.c (earlier packet).
 * - state_utility is unmatched; its prototype is include/game_types.h's.
 */
/* Match candidate: func_800E451C (1,588 bytes); canonical score 0/397, complete
 * extent, no unresolved/unverified sites or errors. E4300 is unchanged context
 * and remains 0/135; it earns no duplicate credit. Outer E398C/E4B58 are real,
 * complete NONMATCH reconstructions. The only kept ABI root is E4B58.
 *
 * Base f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2; context derives from
 * cloud/work/ipa-groups/dot_nearest_path_context_20261005/group.c.
 * Algorithm/declarations derive from historicalsource/rushtherock
 * 845329d7b36f5a384c5625ed9a0aef584ab46139 game/maxpath.c:MP_IntervalPos and
 * func_800E42F8 (revision 3.48, copyright 1996 Atari Corporation).
 *
 * Disclosed restoration: unused nx/ny and the unused reciprocal calculation
 * are actual donor source, already preserved in the historical wave-1 draft.
 * They explain the native frame/local layout and post-call float allocation.
 * The same D_80152768 volatile-s16 declaration is reused from accepted
 * src/blob/func_800EC914.c:66 at the base above; this is qualification
 * provenance, not a claim of proven interrupt or concurrent mutation behavior.
 * No fake caller, new dummy carrier, padding array, or assembly was added.
 * No claim that the original N64 source or every parent detail is recovered.
 * Independent checker owns whole-program acceptance and ROM integration.
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
extern volatile s16 D_80152768;
extern s16 D_80154182, D_801527D8[];
extern s8 D_80152744;
extern s8 D_801543C8;
extern s32 D_80120E50;
extern f32 D_801161D4[], D_801161E4[];
extern f32 D_80153F28[], D_80153F48[], D_80153F68[];
extern f32 D_80153F88[][3], D_80154138[][3];
extern f32 D_80154190, D_801543CC;

extern f32 sqrtf(f32), fabsf(f32);
#pragma intrinsic(sqrtf, fabsf)
extern f32 func_8008B3C8(f32 *);
extern f32 func_8008C768(f32, f32);
extern void func_800A61B0(f32 *, f32 *, f32 *);
extern s16 func_800B9338(s16, s16), func_800DC080(s16, s16);
extern f32 func_800BDD90(f32 *);
extern void camera_blend_between(void *, f32 *);
extern void dispatch_handler(s32);
extern void state_utility(s16, s32, void *);
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
    bestd = 1e20f;
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

/* arcade mp_interval_pos; inlined, the retail 2-word stub remains */
void func_800E42F8(D_8014A250_Record *m, Nav *cp) {
    TrackPt *mp;
    TrackPt *nmp;
    s16 nmpi;
    f32 x;
    f32 y;
    f32 invdist;
    f32 n[2];

    mp = &D_8012E5E8[cp->sel].points[cp->pt];
    nmpi = func_800B9338(cp->pt, cp->sel);
    nmp = &D_8012E5E8[cp->sel].points[nmpi];
    n[0] = nmp->x - mp->x;
    n[1] = nmp->z - mp->z;
    cp->d = func_800BDD90(n);
    invdist = (cp->d > 0.01f) ? 1.0f / cp->d : 0.0f;
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
    f32 x, y, z, nx, ny, score, high_score, delta_spd, mspd;
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

    func_800E42F8(m, cp);
}

#define veccopy(a, b) { (b)[0] = (a)[0]; (b)[1] = (a)[1]; (b)[2] = (a)[2]; }
#define vecsub(a, b, c) { (c)[0] = (a)[0] - (b)[0]; (c)[1] = (a)[1] - (b)[1]; (c)[2] = (a)[2] - (b)[2]; }
#define scalmul(a, s, b) { (b)[0] = (a)[0] * (s); (b)[1] = (a)[1] * (s); (b)[2] = (a)[2] * (s); }

/* arcade avoid_areas */
void func_800E398C(s16 drone_index) {
    s16 i, index, hint, can_we_cheat;
    f32 dist, scale, scale1, scale2, tmp2;
    f32 rpos[3];
    f32 pos[3], tpos[3], own_rwr[3], dir[3], temp[3];
    f32 dir_list[6][3], dir_weight[6];
    f32 cur_rate[3], save_dir[3];
    Nav *cp;
    D_8014A250_Record *m;

    m = &D_8014A250[drone_index];
    cp = &player_array[drone_index].nav;

    veccopy(cp->tgt, tpos);

    vecsub(tpos, m->pos, pos);
    func_800A61B0(pos, rpos, (f32 *) ((u8 *) m + 0x7A0));

    if ((m->f638 ==  0.f ) || (m->speed < .1f)) {
        D_8014A250[drone_index].b7DE = 0;
        rpos[0] =  0.0f ;
        cp->spd = 100.0f;
        veccopy(rpos, cp->tgt);
        for (i = 0; i < 3; i++) {
            D_80153F88[drone_index][i] =  0.0f ;
        }
        return;
    }

    if (m->speed < (f32) (u32) D_8012E5E8[cp->sel].points[cp->pt].flag * 1.4666667f * .5f) {
        rpos[0] *= .2f;
        cp->spd = (f32) (u32) D_8012E5E8[cp->sel].points[cp->pt].flag * 1.4666667f * 1.5f;
        veccopy(rpos, cp->tgt);
        for (i = 0; i < 3; i++) {
            D_80153F88[drone_index][i] = 0;
        }
        D_8014A250[drone_index].b7DE = 0;
        return;
    }

    if (rpos[2] < fabsf(rpos[0] + rpos[0])) {
        veccopy(rpos, cp->tgt);
        for (i = 0; i < 3; i++) {
            D_80153F88[drone_index][i] = 0;
        }
        D_8014A250[drone_index].b7DE = 0;
        return;
    }

    dist = func_8008B3C8(rpos);
    scale = m->speed * m->f634 / dist;
    scalmul(rpos, scale, dir);
    veccopy(dir, save_dir);

    veccopy(D_8014A250[drone_index].pos, own_rwr);

    can_we_cheat = 1;
    scale1 =  1 ;

    for (i = 0; i < D_80152744; i++) {
        index = D_8014A250[i].unk7C6;
        if (index == drone_index) {
            continue;
        }
        m = &D_8014A250[index];

        vecsub(m->pos, own_rwr, pos);
        func_800A61B0(pos, rpos, (f32 *) ((u8 *) &D_8014A250[drone_index] + 0x7A0));

        dist = rpos[0] * rpos[0] + rpos[2] * rpos[2];
        if (dist > 90000.0f) {
            continue;
        }
        if (m->b7CC == 2) {
            if (fabsf(rpos[0]) < 60 && fabsf(rpos[2]) < 200) {
                can_we_cheat = 0;
            }
        }
        if (rpos[2] <  0.f ) {
            continue;
        }
        if (fabsf(rpos[0]) > 8) {
            continue;
        }
        scale2 = 1;
        if (rpos[2] < 20) {
            scale2 = 0.9f;
        } else if (rpos[2] < 100) {
            scale2 = 1.0f - (100 - rpos[2]) * 0.00125000032f;
        }
        if (scale2 < scale1) {
            scale1 = scale2;
        }
    }

    camera_blend_between(&D_8014A250[drone_index], &scale1);

    if (fabsf(dir[0]) * 4 < fabsf(dir[2])) {
        if (dir[0] <  0.0f ) {
            dir[0] += (1.0f - scale1) * 8;
        } else {
            dir[0] -= (1.0f - scale1) * 8;
        }
    } else if (scale1 < 0.9f) {
        if (dir[0] <  0.0f ) {
            dir[0] = (1.0f - scale1) *  4.0f ;
        } else {
            dir[0] = -(1.0f - scale1) *  4.0f ;
        }
    }

    hint = ((s8 *) &D_8012E5E8[cp->sel].points[cp->pt])[7];

    if (D_80153F88[drone_index][0] == 0 && D_80153F88[drone_index][2] == 0) {
        dir[0] = 0;
        cp->spd *= 1.0f - (1.0f - scale1) * .3f;
    } else if (hint == 5 || m->s6C4 >= 0) {
        dir[0] *= .4f;
    } else if (hint == 2 || (D_80153F88[drone_index][0] == 0 && D_80153F88[drone_index][2] == 0)) {
        veccopy(save_dir, dir);
        cp->spd *= 1.0f - (1.0f - scale1) * .3f;
    } else if (hint == 1) {
        if (D_80153F88[drone_index][0] == 0 && D_80153F88[drone_index][2] == 0) {
            veccopy(save_dir, dir);
        } else {
            dir[0] = save_dir[0] * .15f + D_80153F88[drone_index][0] * .85f;
        }
        cp->spd *= 1.0f - (1.0f - scale1) * .1f;
    } else if (hint == 3) {
        dir[0] = save_dir[0] - D_80154138[drone_index][0];
        veccopy(D_80154138[drone_index], save_dir);
    } else if (hint == 4) {
        dir[0] = save_dir[0] - D_80154138[drone_index][0];
    } else {
        dir[0] = dir[0] * .5f + D_80153F88[drone_index][0] * .5f;
    }

    D_8014A250[drone_index].b7DE = can_we_cheat;

    veccopy(dir, cp->tgt);

    veccopy(dir, D_80153F88[drone_index]);

    veccopy(save_dir, D_80154138[drone_index]);
}

#define range_it(v,l,h)		( ((v) < (l)) ? (l) : (((v) > (h)) ? (h) : (v)) )

void func_800E4B50(D_8014A250_Record *m, Nav *cp);
void func_800E42F0(D_8014A250_Record *m, Nav *cp);
void func_800E42E8(D_8014A250_Record *m, Nav *cp);
void func_800E3984(D_8014A250_Record *m, f32 tspd);
void func_800E397C(D_8014A250_Record *m, f32 *pos);

/* arcade MaxPathControls; defined before its helpers as in maxpath.c */
void func_800E4B58(D_8014A250_Record *m)
{
    f32 tspd;
    Nav *cp;

    cp = &player_array[m->unk7C6].nav;
    func_800E4B50(m, cp);
    func_800E42F0(m, cp);
    func_800E42E8(m, cp);

    func_800E398C(m->unk7C6);

    tspd = cp->spd;
    if (m->vy <  0.0f ) {
        tspd *= D_801161D4[D_801543C8];
    } else if (tspd < D_801161E4[D_801543C8]) {
        tspd = D_801161E4[D_801543C8];
    }
    tspd *= m->f7EC;

    func_800E3984(m, tspd);
    func_800E397C(m, cp->tgt);

    m->s7E0 = (D_801543CC - cp->tm > 1.0f) && (cp->pt > 10);

    if (fabsf(cp->a) > 100.0f || fabsf(cp->b) > 100.0f) {
        m->s7E0 = 1;
    }
}

/* arcade MP_FindInterval; inlined, the retail 2-word stub remains */
void func_800E4B50(D_8014A250_Record *m, Nav *cp)
{
    func_800E451C(m, cp, 1);
    if (cp->a >= cp->d) {
        cp->pt = func_800B9338(cp->pt, cp->sel);
        cp->tm = D_801543CC;
        func_800E451C(m, cp, 0);
    } else if (cp->a < 0.0f) {
        cp->pt = func_800DC080(cp->pt, cp->sel);
        func_800E451C(m, cp, 0);
    }
}

/* arcade MP_TargetSpeed; inlined, the retail 2-word stub remains */
void func_800E42F0(D_8014A250_Record *m, Nav *cp)
{
    TrackPt *mp, *nmp;
    s32 nmpi;
    f32 interp, yrel, offsc;

    nmpi = func_800B9338(cp->pt, cp->sel);

    interp = cp->a / cp->d;
    interp = range_it(interp, 0.0f, 1.0f);
    cp->spd = (f32)(u32)D_8012E5E8[cp->sel].points[cp->pt].flag * 1.4666667f + ((f32)(u32)D_8012E5E8[cp->sel].points[nmpi].flag * 1.4666667f - (f32)(u32)D_8012E5E8[cp->sel].points[cp->pt].flag * 1.4666667f) * interp;

    yrel = (cp->b > 0.0f) ? cp->b : -cp->b;
    if (yrel < 0.1f) {
        offsc = 1.0f;
    } else if (yrel > 80.0f) {
        offsc = 0.2f;
    } else {
        offsc = 1.0f - (yrel * yrel * 0.00003125f);
    }
    cp->spd *= offsc;
    if (cp->spd < 28.0f) {
        cp->spd = 28;
    }
}

/* arcade MP_TargetSteerPos; inlined, the retail 2-word stub remains */
void func_800E42E8(D_8014A250_Record *m, Nav *cp)
{
    TrackPt *mp, *nmp;
    s16 nmpi;
    f32 x, y, z, dist, tgtdist, interp;

    mp = &D_8012E5E8[cp->sel].points[cp->pt];
    nmpi = func_800B9338(cp->pt, cp->sel);
    nmp = &D_8012E5E8[cp->sel].points[nmpi];

    tgtdist = 80.0f + cp->a;
    dist = cp->d;
    tgtdist -= dist;
    while (tgtdist > 0.0f) {
        mp = nmp;
        nmpi = func_800B9338(nmpi, cp->sel);
        nmp = &D_8012E5E8[cp->sel].points[nmpi];

        x = nmp->x - mp->x;
        y = nmp->y - mp->y;
        z = nmp->z - mp->z;
        dist = sqrtf(x * x + y * y + z * z);
        tgtdist -= dist;
    }

    if (dist < 1e-5f) {
        dispatch_handler(5);
        state_utility(80, 160, (void *)&D_80120E50);
        interp = 1;
    } else {
        interp = (dist + tgtdist) / dist;
    }
    interp = range_it(interp, 0.0f, 1.0f);

    cp->tgt[0] = (nmp->x - mp->x) * interp + mp->x;
    cp->tgt[1] = (nmp->y - mp->y) * interp + mp->y;
    cp->tgt[2] = (nmp->z - mp->z) * interp + mp->z;
}

/* arcade AdjustSpeed; inlined, the retail 2-word stub remains */
void func_800E3984(D_8014A250_Record *m, f32 tspd)
{
    s32 i, slip, spddir, node, cur_time;
    f32 x, y, z, dspd, delta_speed;

    node = m->unk7C6;

    tspd *= 1.05f;

    if ((D_801543C8 < 2) && ((m->b488 == 40) || (m->b4E4 == 40))) {
        D_80153F48[node] = 0;
        D_80153F68[node] = 0;
    } else {
        x = m->vx;
        y = m->vy;
        z = m->vz;
        dspd = sqrtf(x * x + y * y + z * z);

        delta_speed = tspd - dspd;

        if (delta_speed < 0) {
            delta_speed *= 0.5f;
        }

        D_80153F68[node] += (delta_speed * 0.183f);

        if (D_80153F68[node] < -0.75f) {
            D_80153F48[node] -= (D_80153F68[node] + 0.75f) / 4;
        } else {
            D_80153F48[node] = 0;
        }
    }

    if (D_80153F68[node] < 0) {
        D_80153F68[node] = 0;
    } else if (D_80153F68[node] > 1.0f) {
        D_80153F68[node] = 1.0f;
    }

    if (D_80153F48[node] < 0) {
        D_80153F48[node] = 0;
    } else if (D_80153F48[node] > 1.0f) {
        D_80153F48[node] = 1.0f;
    }

    m->f72C = D_80153F68[node];
    m->f728 = D_80153F48[node];
}

/* arcade AdjustSteer; inlined, the retail 2-word stub remains */
void func_800E397C(D_8014A250_Record *m, f32 *pos)
{
    s32 node;
    f32 angle;

    node = m->unk7C6;

    pos[0] *= m->f634 * 108.43373f;

    angle = func_8008C768(pos[2], pos[0]);

    angle = range_it(angle, 0, 3.1415927f);

    D_80153F28[node] = 1.0f - angle * 0.63661975f;
    m->f720 = D_80153F28[node];
}

