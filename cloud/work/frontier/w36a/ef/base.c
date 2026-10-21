void func_8039EF08(void)
{
    f32 uv[3][3];
    s32 i;
    s32 p;
    f32 carPos[3];
    u8 alpha;
    s32 framesLeft;
    s8 reflectIdx;
    f32 reflectUV[3][3];
    f32 reflectPos[3];
    s32 ghostIdx;
    s32 ghostCar;
    s32 show;
    s32 xx1;
    s32 xx2;
    for (p = 0; p < D_8014A108; p++) {
        for (i = 0; i < D_803BA020[p]; i++) {
            if (((volatile s8 *)D_803B9FD4)[p] == i) {
                D_803B9BC8[p][i].selection += 5.0f * D_8002EB94;
                if (D_803B9BC8[p][i].selection > 1.0f) D_803B9BC8[p][i].selection = 1.0f;
            } else {
                D_803B9BC8[p][i].selection -= 5.0f * D_8002EB94;
                if (D_803B9BC8[p][i].selection < 0.0f) D_803B9BC8[p][i].selection = 0.0f;
            }
            if (D_803BA028[p]) {
                framesLeft = 10 - D_803BA030;
                if (framesLeft <= 1) D_803B9BC8[p][i].angle = 3.6651914f;
                else {
                    D_803B9BC8[p][i].angle -= 3.6651914f;
                    while (D_803B9BC8[p][i].angle > 3.1415927f) D_803B9BC8[p][i].angle -= 6.2831855f;
                    while (D_803B9BC8[p][i].angle <= -3.1415927f) D_803B9BC8[p][i].angle += 6.2831855f;
                    D_803B9BC8[p][i].angle -= D_803B9BC8[p][i].angle / (framesLeft - 1);
                    D_803B9BC8[p][i].angle += 3.6651914f;
                }
            } else if (D_803BA060[p][D_803B28B8[p]] == 11) {
                D_803B9BC8[p][i].angle -= 1.5707964f;
                while (D_803B9BC8[p][i].angle > 3.1415927f) D_803B9BC8[p][i].angle -= 6.2831855f;
                while (D_803B9BC8[p][i].angle <= -3.1415927f) D_803B9BC8[p][i].angle += 6.2831855f;
                if (D_803B9BC8[p][i].angle > 1.5707964f) D_803B9BC8[p][i].angle += CAR_MIN(1.0471976f * D_8002EB94 * 4.0f, 3.1415927f - D_803B9BC8[p][i].angle);
                else if (D_803B9BC8[p][i].angle < -1.5707964f) D_803B9BC8[p][i].angle -= CAR_MIN(1.0471976f * D_8002EB94 * 4.0f, 3.1415927f + D_803B9BC8[p][i].angle);
                else if (D_803B9BC8[p][i].angle > 0.f) D_803B9BC8[p][i].angle -= CAR_MIN(1.0471976f * D_8002EB94 * 4.0f, D_803B9BC8[p][i].angle);
                else D_803B9BC8[p][i].angle += CAR_MIN(1.0471976f * D_8002EB94 * 4.0f, -D_803B9BC8[p][i].angle);
                D_803B9BC8[p][i].angle += 1.5707964f;
            } else {
                D_803B9BC8[p][i].angle += 1.0471976f * D_8002EB94;
                if (D_803B9BC8[p][i].angle >= 6.2831855f) D_803B9BC8[p][i].angle -= 6.2831855f;
            }
            func_8008B32C(D_8011418C, uv, 2.5f);
            func_80090F44(0.34906584f, uv);
            func_80090E9C(D_803B9BC8[p][i].angle, uv);
            if (D_803B9BC8[p][i].car % 13 == 10) {
                if (D_80151AD0 == 1) { carPos[0] = 0.0f; carPos[1] = 3.2f; carPos[2] = 40.5f; }
                else { carPos[0] = 0.0f; carPos[1] = 5.9f; carPos[2] = 40.5f; }
            } else {
                if (D_80151AD0 == 1) { carPos[0] = 0.0f; carPos[1] = 2.6f; carPos[2] = 40.5f; }
                else { carPos[0] = 0.0f; carPos[1] = 5.3f; carPos[2] = 40.5f; }
            }
            alpha = D_803B9BC8[p][i].selection * 255.0f;
            if (D_8014A110 == 6 && D_801164BE == 0)
                func_80390DCC(D_803B9BC8[p][i].car, p, D_80138664[D_8012E67C[p]], D_80138664[D_8012E67C[p]], D_80138664[D_8012E67C[p]], D_803B28B0[p], carPos, uv, alpha);
            else func_80390DCC(D_803B9BC8[p][i].car, p, D_80111604[p + 1][D_803B9BC8[p][i].car % 13], D_80111648[p + 1][D_803B9BC8[p][i].car % 13], D_8011168C[p + 1][D_803B9BC8[p][i].car % 13], D_803B28B0[p], carPos, uv, alpha);
            if (D_8014A108 < 3 && D_8014A110 != 2) {
                reflectIdx = p == 0 ? 2 : 3;
                func_8008B32C(uv, reflectUV, 1.2f);
                func_8009EA68(3.14159265f, reflectUV);
                reflectPos[0] = 1.2f * carPos[0];
                reflectPos[1] = 1.2f * carPos[1];
                reflectPos[2] = 1.2f * carPos[2];
                if (D_803B9BC8[p][i].car % 13 == 10) reflectPos[1] -= 4.0f;
                else reflectPos[1] -= 2.5f;
                D_803B9BC8[reflectIdx][i].car = D_803B9BC8[p][i].car + 26;
                if (D_8014A110 == 6 && D_801164BE == 0)
                    func_80390DCC(D_803B9BC8[reflectIdx][i].car, reflectIdx, D_80138664[D_8012E67C[p]], D_80138664[D_8012E67C[p]], D_80138664[D_8012E67C[p]], D_803B28B0[p], reflectPos, reflectUV, alpha / 1.5f);
                else func_80390DCC(D_803B9BC8[reflectIdx][i].car, reflectIdx, D_80111604[p + 1][D_803B9BC8[p][i].car % 13], D_80111648[p + 1][D_803B9BC8[p][i].car % 13], D_8011168C[p + 1][D_803B9BC8[p][i].car % 13], D_803B28B0[p], reflectPos, reflectUV, alpha / 1.5f);
            }
            if (((volatile s8 *)D_803B9FD4)[p] == i) {
                func_8008B32C(D_8011418C, D_803B9E40[p][0], 1.3f);
                func_8008B32C(D_8011418C, D_803B9E40[p][1], 1.3f);
                func_80090F44(0.34906584f, D_803B9E40[p][0]);
                func_80090F44(0.34906584f, D_803B9E40[p][1]);
                func_80090E9C(D_803B9BC8[p][i].angle, D_803B9E40[p][0]);
                if (D_80151AD0 == 1) {
                    D_803B9E40[p][0][9] = 0.0f;
                    D_803B9E40[p][0][10] = 1.8f;
                    D_803B9E40[p][0][11] = 45.0f;
                    D_803B9E40[p][1][9] = 0.0f;
                    D_803B9E40[p][1][10] = 1.8f;
                    D_803B9E40[p][1][11] = 45.0f;
                } else {
                    D_803B9E40[p][0][9] = 0.0f;
                    D_803B9E40[p][0][10] = 4.8f;
                    D_803B9E40[p][0][11] = 45.0f;
                    D_803B9E40[p][1][9] = 0.0f;
                    D_803B9E40[p][1][10] = 4.8f;
                    D_803B9E40[p][1][11] = 45.0f;
                }
                if (D_8014A110 == 2) {
                    for (ghostIdx = 1; ghostIdx < D_803BA00C + 1; ghostIdx++) {
                        if (!D_80152698[ghostIdx] || D_803BA018[ghostIdx] != (*(*D_80152698[ghostIdx])->active)->car) {
                            if (D_803BA018[ghostIdx] != -1) func_80390DCC(D_803B9BC8[ghostIdx][D_803BA018[ghostIdx]].car, ghostIdx, 0, 0, 0, 0, carPos, uv, 0);
                        }
                        if (!D_80152698[ghostIdx]) continue;
                        ghostCar = (*(*D_80152698[ghostIdx])->active)->car;
                        D_803BA018[ghostIdx] = ghostCar;
                        D_803B9BC8[ghostIdx][ghostCar].car = ghostCar + ghostIdx * 13;
                        D_8011119C[ghostIdx + 1][ghostCar] = (*(*D_80152698[ghostIdx])->active)->component;
                        D_8011157C[ghostIdx + 1][ghostCar] = (*(*D_80152698[ghostIdx])->active)->part;
                        D_801112DC[ghostIdx + 1][ghostCar] = (*(*D_80152698[ghostIdx])->active)->stat0 / 100.0f + 0.75f;
                        D_801113E0[ghostIdx + 1][ghostCar] = (*(*D_80152698[ghostIdx])->active)->stat1 / 100.0f + 0.75f;
                        func_80390DCC(D_803B9BC8[ghostIdx][ghostCar].car, ghostIdx, (*(*D_80152698[ghostIdx])->active)->red, (*(*D_80152698[ghostIdx])->active)->green, (*(*D_80152698[ghostIdx])->active)->blue, 0, carPos, uv, 255);
                    }
                }
            }
        }
        if (D_8014A108 == 1 && D_8014A110 != 2) {
            show = !D_803BA028[0] && (D_803BA060[p][D_803B28B8[p]] == 1 || D_803BA060[p][D_803B28B8[p]] == 2 || D_803BA060[p][D_803B28B8[p]] == 3 || D_803BA060[p][D_803B28B8[p]] == 4 || D_803BA060[p][D_803B28B8[p]] == 5);
            func_8039ED20(show);
            func_8039E444(p);
        }
    }
}
