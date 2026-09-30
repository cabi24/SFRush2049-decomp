s16 input_process_controller(f32 *p1, f32 *p2, f32 *out, Poly *poly, s16 *outIdx, s32 flag, f32 *vcOut, f32 *mat, f32 rad2) {
    s32 res;
    u16 idx[20];
    f32 vp[3];
    f32 vq[3];
    f32 va[3];
    f32 ve[3];
    f32 v0[3];
    f32 vprev[3];
    u32 k;
    u32 n;
    f32 d;
    f32 vd[3];

    res = 1;
    func_800AD650(mat, poly->body);
    n = poly->cnt & 0xF;
    *outIdx = func_800AD5D0((u8 *) (poly->off + D_80152568), n, (s16 *) idx);
    DECODE(v0, (&D_8015201C[idx[0]]));
    va[0] = p1[0] - v0[0];
    va[1] = p1[1] - v0[1];
    va[2] = p1[2] - v0[2];
    func_800A61B0(va, vp, mat);
    if (vp[1] < 0.0f) {
        return 0;
    }
    va[0] = p2[0] - v0[0];
    va[1] = p2[1] - v0[1];
    va[2] = p2[2] - v0[2];
    func_800A61B0(va, vq, mat);
    if (vq[1] > 0.0f) {
        return 0;
    }
    d = vq[1] - vp[1];
    va[0] = vq[0] - vp[0];
    va[2] = vq[2] - vp[2];
    if (d > 0.0f) {
        return 0;
    }
    va[1] = d;
    if (d < 0.0f) {
        va[1] = d;
        vp[0] -= va[0] * (vp[1] / d);
        vp[1] -= d * (vp[1] / d);
        vp[2] -= va[2] * (vp[1] / d);
    }
    DECODE(ve, (&D_8015201C[idx[n - 1]]));
    if ((((ve[2] - vp[2]) * ve[0]) - (ve[2] * (ve[0] - vp[0]))) < 0.0f) {
        if (flag > 0) {
            return 0;
        }
        if (func_800AD4C8(ve, vp, vcOut, rad2) == 0) {
            return 0;
        }
        res = -1;
        goto done;
    }
    DECODE(ve, (&D_8015201C[idx[1]]));
    if (((vp[2] * ve[0]) - (ve[2] * vp[0])) < 0.0f) {
        if (flag > 0) {
            return 0;
        }
        if (func_800AD4C8(ve, vp, vcOut, rad2) == 0) {
            return 0;
        }
        res = -1;
        goto done;
    }
    k = 2;
    if ((u32) n >= 3U) {
        do {
            k += 1;
            vprev[0] = ve[0];
            vprev[2] = ve[2];
            DECODE(ve, (&D_8015201C[idx[k - 1]]));
            va[1] = 0.0f;
            vd[1] = 0.0f;
            
            va[0] = ve[0] - vprev[0];
            va[2] = ve[2] - vprev[2];
            vd[0] = vp[0] - vprev[0];
            vd[2] = vp[2] - vprev[2];
            if (((vd[2] * va[0]) - (va[2] * vd[0])) < 0.0f) {
                if (flag > 0) {
                    return 0;
                }
                if (func_800AD4C8(va, vd, vcOut, rad2) == 0) {
                    return 0;
                }
                res = -1;
                goto done;
            }
        } while (k < (u32) n);
    }
done:
    func_8009E820(vp, out, mat);
    out[0] = v0[0] + out[0];
    out[1] = v0[1] + out[1];
    out[2] = v0[2] + out[2];
    return res;
}
