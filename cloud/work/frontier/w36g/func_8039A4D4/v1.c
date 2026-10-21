void func_8039A4D4(void)
{
    s32 buttonIdx;
    f32 prevPos[3];
    f32 buttonY;
    f32 selectedPos = 0.0f;
    s8 clearScrollSpeed = 1;
    Texture *tex;
    s16 texIdx;
    Color buttonColor = D_803B345C;

    if (!D_803B33C0)
        return;
    if (D_803B342C == 1) {
        for (buttonIdx = 0; buttonIdx < 12; buttonIdx++) {
            if (buttonIdx == 6 || buttonIdx == 8 || buttonIdx == 10)
                model_transform_setup(D_803B30C0[buttonIdx].handle, 0, 15);
            else {
                D_803B30C0[buttonIdx].alpha = 0;
                model_data_load(D_803B30C0[buttonIdx].handle, 0, 15);
            }
        }
        return;
    }
    for (buttonIdx = 0; buttonIdx < 12; buttonIdx++)
        model_transform_setup(D_803B30C0[buttonIdx].handle, 0, 15);
    buttonY = D_803B33D4;
    for (buttonIdx = 0; buttonIdx < 4; buttonIdx++) {
        if (D_803B3414 || D_803B3418) {
            model_data_load(D_803B30C0[buttonIdx].handle, 0, 15);
            if (buttonIdx == 1)
                model_data_load(D_803B30C0[4].handle, 0, 15);
            else if (buttonIdx == 2)
                model_data_load(D_803B30C0[5].handle, 0, 15);
        } else {
            if (buttonIdx == D_803BACF8) {
                selectedPos = buttonY;
                if (D_803B30C0[buttonIdx].angle < 3.1415927f)
                    D_803B30C0[buttonIdx].angle += 12.566371f * D_8002EB94;
                if (D_803B30C0[buttonIdx].angle > 3.1415927f)
                    D_803B30C0[buttonIdx].angle = 3.1415927f;
                if (D_803B33C4)
                    D_803B30C0[buttonIdx].angle = 3.1415927f;
            } else {
                if (D_803B30C0[buttonIdx].angle > 0.0f)
                    D_803B30C0[buttonIdx].angle -= 12.566371f * D_8002EB94;
                if (D_803B30C0[buttonIdx].angle < 0.0f)
                    D_803B30C0[buttonIdx].angle = 0.0f;
                if (D_803B33C4)
                    D_803B30C0[buttonIdx].angle = 0.0f;
            }
            memcpy(prevPos, D_803B30C0[buttonIdx].position, 12);
            D_803B30C0[buttonIdx].position[0] = D_803B33D0 + -100.0f * D_803B33C8;
            D_803B30C0[buttonIdx].position[1] = buttonY;
            D_803B30C0[buttonIdx].position[2] = D_803B33DC;
            if (!D_803B33C4) {
                if (fabsf(D_803B30C0[buttonIdx].position[1] - prevPos[1]) * 60.0f / 10.0f > D_803BA22C) {
                    D_803BA22C = fabsf(D_803B30C0[buttonIdx].position[1] - prevPos[1]) * 60.0f / 10.0f;
                    D_803BA22C = D_803BA22C < 540.0f * D_803B33C8 ? D_803BA22C : 540.0f * D_803B33C8;
                    D_803BA22C = D_803BA22C > 180.0f * D_803B33C8 ? D_803BA22C : 180.0f * D_803B33C8;
                }
                if (D_803B30C0[buttonIdx].position[1] > prevPos[1] + D_803BA22C * D_8002EB94) {
                    D_803B30C0[buttonIdx].position[1] = prevPos[1] + D_803BA22C * D_8002EB94;
                    clearScrollSpeed = 0;
                } else if (D_803B30C0[buttonIdx].position[1] < prevPos[1] - D_803BA22C * D_8002EB94) {
                    D_803B30C0[buttonIdx].position[1] = prevPos[1] - D_803BA22C * D_8002EB94;
                    clearScrollSpeed = 0;
                }
            }
            func_8008B32C((f32 (*)[3])D_8011418C, D_803B30C0[buttonIdx].matrix, D_803B33C8);
            func_800B5898(D_803B30C0[buttonIdx].angle - 1.5707964f, D_803B30C0[buttonIdx].matrix);
            if (D_803B30C0[buttonIdx].angle > 1.5707964f)
                tex = func_800B24EC(D_803B87C8, &texIdx, 0, D_80140BDC - 1, 1);
            else
                tex = func_800B24EC(D_803B87D8, &texIdx, 0, D_80140BDC - 1, 1);
            func_8008D870(D_803B30C0[buttonIdx].handle, (s32)tex, -1);
            if (D_803B30C0[buttonIdx].position[1] > D_803B33D4 + D_803B33CC * D_803B33C8
                || D_803B30C0[buttonIdx].position[1] < D_803B33D8 - D_803B33CC * D_803B33C8)
                D_803B30C0[buttonIdx].alpha = 0;
            else if (D_803B30C0[buttonIdx].position[1] > D_803B33D4)
                D_803B30C0[buttonIdx].alpha = 255.0f - (D_803B30C0[buttonIdx].position[1] - D_803B33D4) * 255.0f / (D_803B33CC * D_803B33C8);
            else if (D_803B30C0[buttonIdx].position[1] < D_803B33D8)
                D_803B30C0[buttonIdx].alpha = 255.0f - (D_803B33D8 - D_803B30C0[buttonIdx].position[1]) * 255.0f / (D_803B33CC * D_803B33C8);
            else
                D_803B30C0[buttonIdx].alpha = 255;
            if (!func_8039A448(buttonIdx))
                D_803B30C0[buttonIdx].alpha = 0;
            buttonColor.channel[3] = D_803B30C0[buttonIdx].alpha;
            func_8008E06C(D_803B30C0[buttonIdx].handle, (s32 *)&buttonColor);
            if (buttonIdx == 1) {
                D_803B30C0[4].angle = D_803B30C0[buttonIdx].angle;
                D_803B30C0[4].position[0] = D_803B33D0 + 100.0f * D_803B33C8;
                D_803B30C0[4].position[1] = D_803B30C0[buttonIdx].position[1];
                D_803B30C0[4].position[2] = D_803B30C0[buttonIdx].position[2];
                func_8008B32C((f32 (*)[3])D_8011418C, D_803B30C0[4].matrix, D_803B33C8);
                func_800B5898(D_803B30C0[4].angle - 1.5707964f, D_803B30C0[4].matrix);
                func_8008D870(D_803B30C0[4].handle, (s32)tex, -1);
                D_803B30C0[4].alpha = D_803B30C0[buttonIdx].alpha;
                func_8008E06C(D_803B30C0[4].handle, (s32 *)&buttonColor);
                if (!func_8039A448(buttonIdx))
                    model_data_load(D_803B30C0[4].handle, 0, 15);
                else if (!D_803B30C0[buttonIdx].alpha)
                    model_data_load(D_803B30C0[4].handle, 0, 15);
                else
                    model_transform_setup(D_803B30C0[4].handle, 0, 15);
            } else if (buttonIdx == 2) {
                D_803B30C0[5].angle = D_803B30C0[buttonIdx].angle;
                D_803B30C0[5].position[0] = D_803B33D0 + 100.0f * D_803B33C8;
                D_803B30C0[5].position[1] = D_803B30C0[buttonIdx].position[1];
                D_803B30C0[5].position[2] = D_803B30C0[buttonIdx].position[2];
                func_8008B32C((f32 (*)[3])D_8011418C, D_803B30C0[5].matrix, D_803B33C8);
                func_800B5898(D_803B30C0[5].angle - 1.5707964f, D_803B30C0[5].matrix);
                func_8008D870(D_803B30C0[5].handle, (s32)tex, -1);
                D_803B30C0[5].alpha = D_803B30C0[buttonIdx].alpha;
                func_8008E06C(D_803B30C0[5].handle, (s32 *)&buttonColor);
                if (!func_8039A448(buttonIdx))
                    model_data_load(D_803B30C0[5].handle, 0, 15);
                else if (!D_803B30C0[buttonIdx].alpha)
                    model_data_load(D_803B30C0[5].handle, 0, 15);
                else
                    model_transform_setup(D_803B30C0[5].handle, 0, 15);
            }
            if (!func_8039A448(buttonIdx))
                model_data_load(D_803B30C0[buttonIdx].handle, 0, 15);
            else if (!D_803B30C0[buttonIdx].alpha) {
                model_data_load(D_803B30C0[buttonIdx].handle, 0, 15);
                buttonY -= D_803B33CC * D_803B33C8;
            } else {
                model_transform_setup(D_803B30C0[buttonIdx].handle, 0, 15);
                buttonY -= D_803B33CC * D_803B33C8;
            }
        }
    }
    memcpy(prevPos, D_803B30C0[7].position, 12);
    D_803B30C0[7].angle += 6.2831855f * D_8002EB94;
    if (D_803B30C0[7].angle > 6.2831855f)
        D_803B30C0[7].angle -= 6.2831855f;
    D_803B30C0[7].position[0] = D_803B33D0 * D_803B33E0 / D_803B33DC + -140.0f * D_803B33C8;
    D_803B30C0[7].position[1] = selectedPos * D_803B33E0 / D_803B33DC;
    D_803B30C0[7].position[2] = D_803B33E0;
    if (!D_803B33C4) {
        if (fabsf(D_803B30C0[7].position[1] - prevPos[1]) * 60.0f / 10.0f > D_803BA228) {
            D_803BA228 = fabsf(D_803B30C0[7].position[1] - prevPos[1]) * 60.0f / 10.0f;
            D_803BA228 = D_803BA228 < 540.0f * D_803B33C8 ? D_803BA228 : 540.0f * D_803B33C8;
            D_803BA228 = D_803BA228 > 180.0f * D_803B33C8 ? D_803BA228 : 180.0f * D_803B33C8;
        }
        if (D_803B30C0[7].position[1] > prevPos[1] + D_803BA228 * D_8002EB94)
            D_803B30C0[7].position[1] = prevPos[1] + D_803BA228 * D_8002EB94;
        else if (D_803B30C0[7].position[1] < prevPos[1] - D_803BA228 * D_8002EB94)
            D_803B30C0[7].position[1] = prevPos[1] - D_803BA228 * D_8002EB94;
        else
            D_803BA228 = 180.0f * D_803B33C8;
    }
    func_8008B32C((f32 (*)[3])D_8011418C, D_803B30C0[7].matrix, D_803B33C8 * 0.4f);
    func_800B5940(D_803B30C0[7].angle, D_803B30C0[7].matrix);
    func_800B5898(1.5707964f, D_803B30C0[7].matrix);
    if (D_803B3414 || D_803B3418) {
        model_data_load(D_803B30C0[7].handle, 0, 15);
        model_data_load(D_803B30C0[9].handle, 0, 15);
    } else {
        model_transform_setup(D_803B30C0[7].handle, 0, 15);
        model_transform_setup(D_803B30C0[9].handle, 0, 15);
    }
    if (clearScrollSpeed == 1)
        D_803BA22C = 0.0f;
    D_803B33C4 = 0;
}
