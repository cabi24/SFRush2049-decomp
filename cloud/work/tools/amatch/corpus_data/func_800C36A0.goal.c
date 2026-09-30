void func_800C36A0(CCar *car, CCol *col) {
    f32 v[3];
    f32 w[3];
    s32 i;
    f32 f12;
    f32 k;
    s32 idx;
    CPoly *poly;

    k = col->k;
    poly = col->poly;
    idx = D_801174C4[col->idx];
    v[0] = k * col->m[3];
    v[1] = k * col->m[4];
    v[2] = col->m[5] * k;
    car->f556[0] = v[0] + car->f556[0];
    car->f556[1] = v[1] + car->f556[1];
    car->f556[2] = v[2] + car->f556[2];
    car->f1940[0] = v[0] + car->f1940[0];
    car->f1940[1] = v[1] + car->f1940[1];
    car->f1940[2] = v[2] + car->f1940[2];
    for (i = 0; i < 4; i++) {
        car->B628[i][0] = v[0] + car->B628[i][0];
        car->B628[i][1] = v[1] + car->B628[i][1];
        car->B628[i][2] = v[2] + car->B628[i][2];
        car->B580[i][0] = v[0] + car->B580[i][0];
        car->B580[i][1] = v[1] + car->B580[i][1];
        car->B580[i][2] = v[2] + car->B580[i][2];
    }
    v[0] = car->A244[idx][1] * car->v76[2] - car->A244[idx][2] * car->v76[1];
    v[1] = car->A244[idx][2] * car->v76[0] - car->A244[idx][0] * car->v76[2];
    v[2] = car->A244[idx][0] * car->v76[1] - car->A244[idx][1] * car->v76[0];
    v[0] = v[0] + car->v64[0];
    v[1] = v[1] + car->v64[1];
    v[2] = v[2] + car->v64[2];
    func_8009E820(v, w, car->mat);
    func_800A61B0(w, v, col->m);
    car->flags2004 |= 0x100000;
    i = poly->type & 0xF;
    f12 = D_80123F54 / car->f1472;
    if (i == 5 || (poly->type & 0x2000) || i == 6) {
        k = car->f1592 * f12 * D_80123F58;
    } else {
        k = car->f1592 * f12 * D_80123F5C;
    }
    w[0] = -v[0] * k;
    w[2] = -v[2] * k;
    if (car->b1996 == 1) {
        k = fabsf((car->mat[6] * col->m[3] + car->mat[7] * col->m[4]) + car->mat[8] * col->m[5]) * D_80123F60 + D_80123F64;
    } else {
        k = fabsf((car->mat[6] * col->m[3] + car->mat[7] * col->m[4]) + car->mat[8] * col->m[5]) * D_80123F68 + D_80123F6C;
    }
    w[1] = -v[1] * f12 * car->f1592 * k;
    func_8009E820(w, v, col->m);
    func_800A61B0(v, w, car->mat);
    if ((i == 5 || i == 6) && car->w1548[idx] == 8 && fabsf(w[2]) < 4000.0f && fabsf(w[1]) < 4000.0f) {
        car->f300 += w[2];
        car->A196[idx][0] += w[0];
    } else {
        car->A196[idx][0] += w[0];
        car->A196[idx][1] += w[1];
        car->A196[idx][2] += w[2];
    }
}