void func_800E92C8(Cr *car, f32 *camoff, f32 ab, f32 AB) {
    f32 vec[3], res[3], temp[3], A, a, B, b, fact, magvel, uvs[3][3];
    s16 fixed_cam, flag;
    char buf[8];

    s32 pl;

    pl = car->player;
    if (car->mode == 8)
        magvel = 0;
    else
        magvel = sqrtf(car->vz*car->vz + car->vx*car->vx);

    if (magvel <= MAX_VEL) {
        vec[0] = 0.0f;
        vec[1] = AB;
        vec[2] = -ab;
        if (car->mode == 8) {
            temp[0] = vec[0];
            temp[1] = vec[1];
            temp[2] = vec[2];
        } else
            func_8009E820(vec, temp, car->m);
        temp[1] = fabsf(temp[1]);
    }

    if (magvel > 1) {
        if (car->mode == 4) {
            vec[0] = car->f_ac;
            vec[1] = car->f_a8;
            vec[2] = 0.0f;
            func_8009E820(vec, res, car->m);
            res[0] = -res[0];
            res[2] = -res[2];
        } else {
            res[0] = car->vx;
            res[1] = car->vy;
            res[2] = car->vz;
            if (ab < 0)
                res[1] = -res[1];
            func_800A61B0(res, vec, car->m);
            vec[2] = fabsf(vec[2]);
            func_8009E820(vec, res, car->m);
            res[0] = -res[0];
            res[2] = -res[2];
        }

        fact = ab / magvel;
        res[0] *= fact;
        res[1] *= fact;
        res[2] *= fact;

        a = res[1];
        b = sqrtf(res[0]*res[0] + res[2]*res[2]);

        if (b > .001f) {
            A = -a * AB / ab;
            fact = (b - A) / b;
            res[0] *= fact;
            res[2] *= fact;
        }

        B = b * AB / ab;
        res[1] = fabsf(B - res[1]);

        if (magvel < MAX_VEL)
            func_800CFDEC(temp, res, 3, 1, MAX_VEL, magvel, res);
    } else {
        res[0] = temp[0];
        res[1] = temp[1];
        res[2] = temp[2];
    }

    if (car->mode == 8)
        D_80152720[pl] = 0.0f;
    else if (car->mode == 4)
        D_80152720[pl] = 1.0f - magvel / MAX_VEL;
    else if (magvel < MAX_VEL)
        D_80152720[pl] = 1.0f - magvel * .01f;
    else
        D_80152720[pl] = 0.0f;

    camoff[0] = res[0];
    camoff[1] = res[1];
    camoff[2] = res[2];
}

void func_800E95DC(s16 mode, Cr *car, f32 *pos, s32 uvs) {
    V3 delta[4];
    f32 res[3], pos2[3];
    s32 pl;
    f32 k;

    pl = car->player;
    if (mode == 0) {
        D_8012E690[pl].x = pos[0];
        D_8012E690[pl].y = pos[1];
        D_8012E690[pl].z = pos[2];
        D_80150B70[pl].v84[0] = pos[0];
        D_80150B70[pl].v84[1] = pos[1];
        D_80150B70[pl].v84[2] = pos[2];
        res[0] = car->pos[0];
        res[1] = car->pos[1];
        res[2] = car->pos[2];
        pos[0] = pos[0] - res[0];
        pos[1] = pos[1] - res[1];
        pos[2] = pos[2] - res[2];
        vector_normalize_length(pos, &D_80150B70[pl].n60);
        D_8012E6C8[pl] = 0;
        D_8012E6E8[pl] = 0.0f;
        return;
    }
    switch (D_8012E6C8[pl]) {
    case 0:
        func_800E92C8(car, res, D_801526F8[pl], D_801526E0[pl]);
        res[0] = res[0] + pos[0];
        res[1] = res[1] + pos[1];
        res[2] = res[2] + pos[2];
        k = .1f * D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]];
        delta[pl].x = k * (res[0] - D_8012E690[pl].x);
        delta[pl].y = k * (res[1] - D_8012E690[pl].y);
        delta[pl].z = k * (res[2] - D_8012E690[pl].z);
        k = D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]];
        pos2[0] = car->pos[0] - car->m[6] * 40.0f;
        pos2[1] = car->pos[1] + 5.0f;
        pos2[2] = car->pos[2] - car->m[8] * 40.0f;
        delta[pl].x = k * (pos2[0] - D_8012E690[pl].x);
        delta[pl].y = k * (pos2[1] - D_8012E690[pl].y);
        delta[pl].z = k * (pos2[2] - D_8012E690[pl].z);
        break;
    case 1:
        func_800E92C8(car, res, D_801526F8[pl], D_801526E0[pl]);
        res[0] = res[0] + pos[0];
        res[1] = res[1] + pos[1];
        res[2] = res[2] + pos[2];
        k = D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]];
        delta[pl].x = k * (res[0] - D_8012E690[pl].x);
        delta[pl].y = k * (res[1] - D_8012E690[pl].y);
        delta[pl].z = k * (res[2] - D_8012E690[pl].z);
        break;
    case 2:
        func_800E9234(car);
        if (!(state_word_a & 8)) {
            s8 v;
            v = 2;
            if (car->q->h[0]->f2c != 0)
                v = func_800CDE38(car->q->h);
            car->mode = v;
            car->mode2 = v;
            if (car->mode == 1) {
                D_801526A8[car->player].x = 0.0f;
                D_801526A8[car->player].y = 0.0f;
                D_801526A8[car->player].z = 0.0f;
            }
        }
        break;
    }
    if (D_8012E6C8[pl] == 0 || D_8012E6C8[pl] == 1) {
        pos2[0] = delta[pl].x + D_8012E690[pl].x;
        pos2[1] = delta[pl].y + D_8012E690[pl].y;
        pos2[2] = delta[pl].z + D_8012E690[pl].z;
        res[0] = pos2[0] - pos[0];
        res[1] = pos2[1] - pos[1];
        res[2] = pos2[2] - pos[2];
        D_80152720[pl] = 0.0f;
        func_800E8D50(car, pos, uvs, res);
        D_80150B70[pl].v84[1] += 4.0f;
        D_8012E6E8[pl] += D_8002EB94;
        if (D_801108C8[D_8012E6C8[pl]] <= D_8012E6E8[pl]) {
            D_8012E6C8[pl]++;
            D_8012E6E8[pl] = 0.0f;
            D_8012E690[pl].x = D_80150B70[pl].v84[0];
            D_8012E690[pl].y = D_80150B70[pl].v84[1];
            D_8012E690[pl].z = D_80150B70[pl].v84[2];
        }
    }
}

void func_800EA108(Cr *car, f32 *pos, f32 *uvs) {
    s32 i, j;
    f32 rpos[3], res[3];
    s32 slot;

    slot = car->player;
    func_8008D6FC(car->s_f6, pos, uvs);
    D_80152708[slot] = .13f;
    func_800E92C8(car, res, 30, 16);
    for (i = 0; i < 3; i++)
        D_80150B70[slot].v84[i] = (D_80150B70[slot].v84[i]*(1.0f-D_80152708[slot]) + (pos[i]+res[i])*D_80152708[slot]);
    for (i = 0; i < 3; i++)
        rpos[i] = pos[i] - D_80150B70[slot].v84[i];
    vector_normalize_length(rpos, &D_80150B70[slot].n60);
}
