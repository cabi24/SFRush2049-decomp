s16 func_800C3AD0(f32 *pt, f32 *wp, Poly *poly, s16 *outIdx, f32 *q, f32 *mat, f32 *bound, f32 zmin) {
    f32 va[3];
    f32 vb[3];
    f32 vc[3];
    f32 vd[3];
    f32 ve[3];
    u16 idx[20];
    volatile f32 f1;
    volatile f32 f2;
    u32 k;
    u32 n;
    s32 res;

    n = poly->cnt & 0xF;
    res = 1;
    *outIdx = func_800AD5D0((u8 *) (poly->off + D_80152568), n, (s16 *) idx);
    func_800AD650(mat, poly->body);
    DECODE(vb, (&D_8015201C[idx[0]]));
    va[0] = wp[0] - vb[0];
    va[1] = wp[1] - vb[1];
    va[2] = wp[2] - vb[2];
    func_800A61B0(va, pt, mat);
    if ((pt[1] <= zmin) || (*bound < pt[1])) {
        return 0;
    }
    DECODE(vb, (&D_8015201C[idx[n - 1]]));
    if ((((vb[2] - pt[2]) * vb[0]) - (vb[2] * (vb[0] - pt[0]))) < 0.0f) {
        if (func_800AD4C8(vb, pt, vc, D_80123F70) == 0) {
            return 0;
        }
        res = -1;
        goto done;
    }
    DECODE(vb, (&D_8015201C[idx[1]]));
    if (((pt[2] * vb[0]) - (vb[2] * pt[0])) < 0.0f) {
        if (func_800AD4C8(vb, pt, vc, D_80123F74) == 0) {
            return 0;
        }
        res = -1;
        goto done;
    }
    k = 2;
    if ((u32) n >= 3U) {
        do {
            k += 1;
            ve[0] = vb[0];
            ve[2] = vb[2];
            DECODE(vb, (&D_8015201C[idx[k - 1]]));
            va[1] = 0.0f;
            f1 = ve[0];
            va[0] = f2 = vb[0] - ve[0];
            va[2] = vb[2] - ve[2];
            vd[1] = 0.0f;
            vd[0] = pt[0] - f1;
            vd[2] = pt[2] - ve[2];
            if (((vd[2] * f2) - (va[2] * vd[0])) < 0.0f) {
                if (func_800AD4C8(va, vd, vc, D_80123F78) == 0) {
                    return 0;
                }
                res = -1;
                goto done;
            }
        } while (k < (u32) n);
    }
done:
    *bound = pt[1];
    if ((q != NULL) && (((poly->type & 0xF) == 5) || ((poly->type & 0xF) == 6))) {
        DECODE(vb, (&D_8015201C[idx[0]]));
        va[0] = q[0] - vb[0];
        va[1] = q[1] - vb[1];
        va[2] = q[2] - vb[2];
        func_800A61B0(va, vb, mat);
        if ((vb[1] <= *bound) || (vb[1] < D_80123F7C)) {
            return 0;
        }
    }
    return res;
}
