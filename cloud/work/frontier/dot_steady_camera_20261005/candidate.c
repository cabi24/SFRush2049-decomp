/* func_800E95DC, 0x800E95DC..0x800E9C70 (1,684 bytes).
 * N64 per-player descendant of arcade camera.c:steady_move_cam.
 * Compile only in the genuine accepted func_800E92C8 group (verify.py).
 * IDO 5.3 -g0 -O3 -mips2 -G 0 -non_shared, normal -r4300_mul.
 * The donor performs vecsub followed by scalmul. Preserve those two phases:
 * combining each component into one multiply expression changes scheduling.
 * Existing type/global declarations and accepted context bodies are unchanged.
 */
void func_800E95DC(s16 mode, Cr *car, f32 *pos, s32 uvs) {
    V3 delta[4];
    f32 res[3], pos2[3];
    s32 pl;

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
        func_800E92C8(car, D_801526F8[pl], D_801526E0[pl], res);
        res[0] = pos[0] + res[0];
        res[1] = pos[1] + res[1];
        res[2] = pos[2] + res[2];
        delta[pl].x = res[0] - D_8012E690[pl].x;
        delta[pl].y = res[1] - D_8012E690[pl].y;
        delta[pl].z = res[2] - D_8012E690[pl].z;
        delta[pl].x = delta[pl].x * (.1f * D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]);
        delta[pl].y = delta[pl].y * (.1f * D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]);
        delta[pl].z = delta[pl].z * (.1f * D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]);
        pos2[0] = car->pos[0] - car->m[6] * 40.0f;
        pos2[1] = car->pos[1] + 5.0f;
        pos2[2] = car->pos[2] - car->m[8] * 40.0f;
        delta[pl].x = pos2[0] - D_8012E690[pl].x;
        delta[pl].y = pos2[1] - D_8012E690[pl].y;
        delta[pl].z = pos2[2] - D_8012E690[pl].z;
        delta[pl].x = delta[pl].x * (D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]);
        delta[pl].y = delta[pl].y * (D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]);
        delta[pl].z = delta[pl].z * (D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]);
        break;
    case 1:
        func_800E92C8(car, D_801526F8[pl], D_801526E0[pl], res);
        res[0] = pos[0] + res[0];
        res[1] = pos[1] + res[1];
        res[2] = pos[2] + res[2];
        delta[pl].x = res[0] - D_8012E690[pl].x;
        delta[pl].y = res[1] - D_8012E690[pl].y;
        delta[pl].z = res[2] - D_8012E690[pl].z;
        delta[pl].x = delta[pl].x * (D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]);
        delta[pl].y = delta[pl].y * (D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]);
        delta[pl].z = delta[pl].z * (D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]);
        break;
    case 2:
        func_800E9234(car);
        if (!(state_word_a & 8)) {
            s32 v;
            if (car->q->h[0]->f2c != 0)
                v = func_800CDE38(car->q->h);
            else
                v = 2;
            car->mode = v;
            car->mode2 = v;
            if (car->mode == 1) {
                D_801526A8[car->player].x = 0;
                D_801526A8[car->player].y = 0;
                D_801526A8[car->player].z = 0;
            }
        }
        break;
    }
    if (D_8012E6C8[pl] == 0 || D_8012E6C8[pl] == 1) {
        pos2[0] = D_8012E690[pl].x + delta[pl].x;
        pos2[1] = D_8012E690[pl].y + delta[pl].y;
        pos2[2] = D_8012E690[pl].z + delta[pl].z;
        res[0] = pos2[0] - pos[0];
        res[1] = pos2[1] - pos[1];
        res[2] = pos2[2] - pos[2];
        D_80152720[pl] = 0;
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
