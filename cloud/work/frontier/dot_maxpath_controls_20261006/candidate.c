/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* NONMATCH research: func_800E4B58, native MaxPathControls adaptation.
 * Arcade source: historicalsource/rushtherock 845329d7b36f5a384c5625ed9a0aef584ab46139,
 * game/maxpath.c revision 3.48, copyright 1996 Atari Corporation.
 * Real helper boundaries: MP_FindInterval, MP_TargetSpeed, MP_TargetSteerPos,
 * AdjustSpeed, AdjustSteer. Names below do not assign retail stub identities.
 * The unused flag in MP_FindInterval is present in that actual donor body.
 * Other unused arcade declarations were omitted; N64 source-history details
 * remain hypotheses. Helpers are internal and called by the actual parent.
 * No stand-ins, added ABI arguments, inline assembly, or invented padding.
 * Requires current nearest-path caller/callee declarations, supplied by repro.py.
 * Observed NONMATCH: 519/567 differing words, 539 emitted words, 304-byte frame,
 * 20 unverified own-rodata words. Neither context nor helper stubs are claimed.
 */
void mp_find_interval_n64(D_8014A250_Record *car, Nav *nav) {
    s16 flag;
    func_800E451C(car, nav, 1);
    if (nav->d <= nav->a) {
        nav->pt = func_800B9338(nav->pt, nav->sel);
        nav->tm = D_801543CC;
        func_800E451C(car, nav, 0);
    } else if (nav->a < 0.0f) {
        nav->pt = func_800DC080(nav->pt, nav->sel);
        func_800E451C(car, nav, 0);
    }
}

void mp_target_speed_n64(D_8014A250_Record *car, Nav *nav) {
    s16 idx;
    TrackPt *pts;
    f32 t, f0, f1, ax, gg;
    idx = func_800B9338(nav->pt, nav->sel);
    t = nav->a / nav->d;
    if (t < 0.0f) {
        t = 0.0f;
    } else if (1.0f < t) {
        t = 1.0f;
    }
    pts = D_8012E5E8[nav->sel].points;
    f0 = (f32) (u32) pts[nav->pt].flag * 1.4666667f;
    f1 = (f32) (u32) pts[idx].flag * 1.4666667f;
    nav->spd = ((f1 - f0) * t) + f0;
    ax = nav->b;
    if (0.0f < ax) {
        ax = ax;
    } else {
        ax = -ax;
    }
    if (ax < 0.100000001f) {
        gg = 1.0f;
    } else if (80.0f < ax) {
        gg = 0.200000003f;
    } else {
        gg = 1.0f - ((ax * ax) * 3.12500015e-05f);
    }
    nav->spd = nav->spd * gg;
    if (nav->spd < 28.0f) {
        nav->spd = 28.0f;
    }
}

void mp_target_position_n64(D_8014A250_Record *car, Nav *nav) {
    s16 sel, idx;
    TrackPt *s1, *s2;
    f32 rem, seg, dx, dy, dz, w;
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
    if (seg < 9.99999975e-06f) {
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
}

void mp_adjust_speed_n64(D_8014A250_Record *car, f32 tspd) {
    s32 node;
    f32 x, y, z, dspd, delta_speed;
    s8 dif;

    node = car->unk7C6;
    dif = D_801543C8;
    if (car->vy < 0.0f) {
        tspd *= D_801161D4[dif];
    } else if (tspd < D_801161E4[dif]) {
        tspd = D_801161E4[dif];
    }
    tspd *= car->f7EC;
    tspd *= 1.04999995f;
    if ((dif < 2) && ((car->b488 == 40) || (car->b4E4 == 40))) {
        D_80153F48[node] = 0.0f;
        D_80153F68[node] = 0.0f;
    } else {
        x = car->vx;
        y = car->vy;
        z = car->vz;
        dspd = sqrtf(x*x + y*y + z*z);
        delta_speed = tspd - dspd;
        if (delta_speed < 0.0f) {
            delta_speed *= 0.5f;
        }
        D_80153F68[node] += delta_speed * 0.183f;
        if (D_80153F68[node] < -0.75f) {
            D_80153F48[node] -= (D_80153F68[node] + 0.75f) / 4.0f;
        } else {
            D_80153F48[node] = 0.0f;
        }
    }
    if (D_80153F68[node] < 0.0f) {
        D_80153F68[node] = 0.0f;
    } else if (D_80153F68[node] > 1.0f) {
        D_80153F68[node] = 1.0f;
    }
    if (D_80153F48[node] < 0.0f) {
        D_80153F48[node] = 0.0f;
    } else if (D_80153F48[node] > 1.0f) {
        D_80153F48[node] = 1.0f;
    }
    car->f72C = D_80153F68[node];
    car->f728 = D_80153F48[node];
}

void mp_adjust_steer_n64(D_8014A250_Record *car, f32 *pos) {
    s16 n;
    f32 ang, f0, f1;
    n = car->unk7C6;
    pos[0] = pos[0] * (car->f634 * 108.433731f);
    ang = func_8008C768(pos[2], pos[0]);
    if (ang < 0.0f) {
        f1 = 0.0f;
    } else {
        f1 = 3.14159274f;
        if (!(f1 < ang)) {
            f1 = ang;
        }
    }
    f0 = 1.0f - (f1 * 0.636619747f);
    D_80153F28[n] = f0;
    car->f720 = f0;
}

void func_800E4B58(D_8014A250_Record *car) {
    f32 seg;
    Nav *nav;

    nav = (Nav *) ((u8 *) &player_array[car->unk7C6] + 0x314);
    mp_find_interval_n64(car, nav);
    mp_target_speed_n64(car, nav);
    mp_target_position_n64(car, nav);
    func_800E398C(car->unk7C6);
    seg = nav->spd;
    mp_adjust_speed_n64(car, seg);
    mp_adjust_steer_n64(car, nav->tgt);
    car->s7E0 = ((D_801543CC - nav->tm) > 1.0f) && (nav->pt > 10);
    if ((100.0f < fabsf(nav->a)) || (100.0f < fabsf(nav->b))) {
        car->s7E0 = 1;
    }
}
