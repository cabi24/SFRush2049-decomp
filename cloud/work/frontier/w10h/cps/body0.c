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
    u32 k;
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
    f32 c1;
    Poly *old;
    f32 c2;
    f32 a;
    f32 b;
    s32 i;
    s32 j;
    PCar *pc;
    f32 *p0;
    f32 *p1;

    conv = 0;
    n = poly->cnt & 0xF;
    func_800AD5D0((u8 *) (poly->off + D_80152568), n, (s16 *) idx);
    v = &D_8015201C[idx[0]];
    DECODE(org, v);
    func_800AD650(poly->body, mtx);
    for (i = 0; i < 4; i++) {
        d[0] = car->B628[D_801174C4[i]][0] - org[0];
        d[1] = car->B628[D_801174C4[i]][1] - org[1];
        d[2] = car->B628[D_801174C4[i]][2] - org[2];
        func_800A61B0(d, pts[i], mtx);
    }
    type = poly->type & 0xF;
    if (type == 5 || type == 6) {
        dz = -3.0f;
    } else if (mtx[3] * car->mat[3] + mtx[4] * car->mat[4] + car->mat[5] * mtx[5] < -0.707f) {
        dz = -4.5f;
    } else {
        dz = -0.5f;
    }
    func_800A61B0(&car->mat[3], d, mtx);
    for (i = 0; i < 4; i++) {
        pts[i + 4][0] = d[0] * dz + pts[i][0];
        pts[i + 4][1] = d[1] * dz + pts[i][1];
        pts[i + 4][2] = d[2] * dz + pts[i][2];
    }
    best = 0.0f;
    bestk = -1;
    for (e = 0; e < 8; e++) {
        ka = D_801174CC[e];
        kb = D_801174CC[e + 8];
        p0 = pts[ka];
        p1 = pts[kb];
        if (p0[1] * p1[1] < 0.0f) {
            if (conv == 0) {
                conv = 1;
                for (j = n - 1; j > 0; j--) {
                    v = &D_8015201C[idx[j]];
                    pv[j][0] = (f32) ((v->x << 5) + ((v->w & 0x7C00) >> 10)) * 0.03125f;
                    pv[j][1] = (f32) ((v->z << 5) + (v->w & 0x1F)) * 0.03125f;
                }
            }
            dx = p1[0] - p0[0];
            dz = p1[2] - p0[2];
            if (p1[1] != p0[1]) {
                dy = p1[1] - p0[1];
                da = p0[1];
                db = p1[1];
                u = fabsf(da / dy);
                hx = p0[0] + dx * u;
                hz = p0[2] + dz * u;
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
                    if (dz != 0.0f) {
                        u = -hz / dz;
                        if (u > 0.0f && u < t) {
                            t = u;
                        }
                    }
                    for (k = 2; k < n; k++) {
                        ex = pv[k][0] - px;
                        ez = pv[k][1] - pz;
                        c1 = ex * (hz - pz);
                        c2 = (hx - px) * ez;
                        if (c1 < c2) {
                            goto next;
                        }
                        a = ex * dz;
                        b = ez * dx;
                        if (b != a) {
                            u = (c2 - c1) / (a - b);
                            if (u > 0.0f && u < t) {
                                t = u;
                            }
                        }
                        px = pv[k][0];
                        pz = pv[k][1];
                    }
                    c1 = pz * hx;
                    c2 = px * hz;
                    if (!(c1 < c2)) {
                        a = px * dz;
                        b = pz * dx;
                        type = poly->type & 0xF;
                        if (b != a) {
                            u = (c1 - c2) / (a - b);
                            if (u > 0.0f && u < t) {
                                t = u;
                            }
                        }
                        if (type == 4) {
                            pc = &((PCar *) player_array)[car->pidx];
                            if (pc->f359 == 0 && pc->f358 == 0 && car->f6C4 == -1) {
                                if (gameplay_mode == 6 || gameplay_mode == 4) {
                                    func_803914B4(car->pidx, car->pidx, -1, car->pidx);
                                }
                                car->b1600 = 1;
                            }
                        } else if (type == 7) {
                            pc = &((PCar *) player_array)[car->pidx];
                            if (pc->f359 == 0 && pc->f358 == 0 && car->f6C4 == -1) {
                                car->b1741 = 1;
                                func_800C54F0(car->pidx, 1);
                            }
                        } else if (type == 3) {
                            pc = &((PCar *) player_array)[car->pidx];
                            if (pc->f359 == 0 && pc->f358 == 0 && car->f6C4 == -1) {
                                car->b1741 = 1;
                                func_800C54F0(car->pidx, 1);
                                if (D_80153E88[car->pidx].kind == 6) {
                                    func_800B61A8(21, 0, 1, 2);
                                }
                            }
                            return;
                        }
                        t *= dy;
                        if (t < 0.0f) {
                            t = -t;
                        }
                        if (best < t) {
                            k = kb;
                            if (-da / db < 1.0f) {
                                k = ka;
                            }
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
        old = col->poly;
        if (old == NULL) {
            col->poly = poly;
            math_utility(mtx, col->m);
            col->idx = bestk;
            col->edge = beste;
            col->k = best;
        } else if (col->k < best) {
            func_800C36A0(car, col);
            col->poly = NULL;
            camera_play_script(car, poly, col);
        } else {
            col->poly = poly;
            math_utility(mtx, col->m);
            col->idx = bestk;
            col->edge = beste;
            col->k = best;
            func_800C36A0(car, col);
            col->poly = NULL;
            camera_play_script(car, old, col);
        }
    }
}
