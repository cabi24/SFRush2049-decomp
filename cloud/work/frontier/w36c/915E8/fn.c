/* func_803915E8 = updateSelectButtons@recordscr.c (MAT3 GC 0x80084f38, size 0xa60). Direct typed port.
 * Globals (N64 address = MAT3 name): D_803B5F28 buttonInfo[23] (64-byte tMenuButton: buttonType, objIdx, angle,
 * mat.uvs[3][3], mat.uvs[3] position, alpha), D_803B64E8 gButtonScale, D_803B64EC gButtonSpace, D_803B64F0 buttonX,
 * D_803B64F4 maxY, D_803B64F8 minY, D_803B64FC buttonZ, D_803B6500 cursorZ, D_803BA7D8 cursorSpeed,
 * D_803BA7DC scrollSpeed, D_803BA7D4 buttonsJustInited, D_803BA7F0 gSelectMenuVisible[11], D_803BA808
 * gSelectMenuSelect, D_803BA80C gSelectMenuClear, D_803BA84D gDeleteConfirm, D_803BA854 gDeleteNotify,
 * D_8002EB94 gFrameSeconds (volatile), D_80140BDC gNumModels (volatile), D_8011418C unitmatrix.
 * Calls: model_transform_setup = MBOX_ShowObject, model_data_load = MBOX_HideObject, func_800B24EC =
 * MBOX_FindTexture_WORK, func_8008D870 = ZOID_SetTexture, func_8008E06C = ZOID_SetColor, func_800B5940 = WRollUV,
 * func_800B5898 = WPitchUV, func_8008B32C = ScaleUV. */
extern f32 D_803B64E8, D_803B64EC, D_803B64F0, D_803B64F4, D_803B64F8, D_803B64FC, D_803B6500;
extern f32 D_803BA7D8, D_803BA7DC;
extern volatile f32 D_8002EB94;
extern volatile u8 D_80140BDC;
extern f32 D_8011418C[3][3];
extern s32 D_803BA7D4, D_803BA808;
extern s8 D_803BA7F0[], D_803BA80C, D_803BA84D, D_803BA854;
extern void *memcpy(void *, const void *, u32);
extern void model_data_load(s32, s32, s32), model_transform_setup(s32, s32, s32);
extern void *func_800B24EC(char *, s16 *, s32, s8, s32);
extern void func_8008D870(s16, void *, s32);
extern void func_8008E06C(s16, u8 *);
extern void func_800B5898(f32, f32 (*)[3]), func_800B5940(f32, f32 (*)[3]);
extern void func_8008B32C(f32 (*)[3], f32 (*)[3], f32);
extern f32 fabsf(f32);
#pragma intrinsic(fabsf)

void func_803915E8(void)
{
    s32 buttonIdx;
    s32 i;
    s32 unusedA, unusedB;
    f32 prevPos[3];
    s32 gSelectMenuCount;
    s32 whichItem;
    f32 buttonY;
    f32 selectedPos;
    s8 clearScrollSpeed = 1;
    void *tex;
    s16 texIdx;
    u8 buttonColor[4] = {255, 255, 255, 255};

    if (D_803BA84D == 1 || D_803BA854 == 1) {
        for (buttonIdx = 0; buttonIdx < 23; buttonIdx++) {
            if (buttonIdx == 17 || buttonIdx == 19 || buttonIdx == 21 || buttonIdx == 22)
                model_transform_setup(D_803B5F28[buttonIdx].fields[1], 0, 15);
            else model_data_load(D_803B5F28[buttonIdx].fields[1], 1, 15);
        }
    } else {
        for (buttonIdx = 0; buttonIdx < 23; buttonIdx++) {
            if ((buttonIdx > 11 && buttonIdx < 13) || buttonIdx == 15 || buttonIdx == 16)
                model_data_load(D_803B5F28[buttonIdx].fields[1], 1, 15);
            else model_transform_setup(D_803B5F28[buttonIdx].fields[1], 0, 15);
        }
        gSelectMenuCount = 0;
        whichItem = 0;
        for (i = 0; i < 11; i++) {
            if (D_803BA7F0[i]) {
                gSelectMenuCount++;
                if (i < D_803BA808) whichItem++;
            }
        }
        if (D_803B64E8 * ((gSelectMenuCount - 1) * D_803B64EC) > D_803B64F4 - D_803B64F8)
            selectedPos = D_803B64F4 - whichItem * (D_803B64F4 - D_803B64F8) / (gSelectMenuCount - 1);
        else selectedPos = D_803B64F4 - D_803B64E8 * (whichItem * D_803B64EC);
        memcpy(prevPos, D_803B5F28[18].position, 12);
        D_803B5F28[18].angle += 6.2831855f * D_8002EB94;
        if (D_803B5F28[18].angle > 6.2831855f) D_803B5F28[18].angle -= 6.2831855f;
        memcpy(D_803B5F28[18].matrix, D_8011418C, 36);
        D_803B5F28[18].position[0] = D_803B64F0 * D_803B6500 / D_803B64FC + -165.0f * D_803B64E8;
        D_803B5F28[18].position[1] = selectedPos * D_803B6500 / D_803B64FC;
        D_803B5F28[18].position[2] = D_803B6500;
        if (!D_803BA7D4) {
            if (fabsf(D_803B5F28[18].position[1] - prevPos[1]) * 60 * D_803B64E8 / 10 > D_803BA7D8) {
                D_803BA7D8 = fabsf(D_803B5F28[18].position[1] - prevPos[1]) * 60 * D_803B64E8 / 10;
                D_803BA7D8 = D_803BA7D8 < 540 * D_803B64E8 ? D_803BA7D8 : 540 * D_803B64E8;
                D_803BA7D8 = D_803BA7D8 > 180 * D_803B64E8 ? D_803BA7D8 : 180 * D_803B64E8;
            }
            if (D_803B5F28[18].position[1] > prevPos[1] + D_803BA7D8 * D_8002EB94)
                D_803B5F28[18].position[1] = prevPos[1] + D_803BA7D8 * D_8002EB94;
            else if (D_803B5F28[18].position[1] < prevPos[1] - D_803BA7D8 * D_8002EB94)
                D_803B5F28[18].position[1] = prevPos[1] - D_803BA7D8 * D_8002EB94;
            else D_803BA7D8 = 180 * D_803B64E8;
        }
        func_800B5940(D_803B5F28[18].angle, D_803B5F28[18].matrix);
        func_800B5898(1.5707964f, D_803B5F28[18].matrix);
        func_8008B32C(D_803B5F28[18].matrix, D_803B5F28[18].matrix, D_803B64E8 * 0.4f);
        buttonY = selectedPos + D_803B64E8 * (whichItem * D_803B64EC);
        for (buttonIdx = 0; buttonIdx < 11; buttonIdx++) {
            if (buttonIdx == D_803BA808 && !D_803BA80C) {
                if (D_803B5F28[buttonIdx].angle < 3.1415927f) D_803B5F28[buttonIdx].angle += 12.566371f * D_8002EB94;
                if (D_803B5F28[buttonIdx].angle > 3.1415927f) D_803B5F28[buttonIdx].angle = 3.1415927f;
                if (D_803BA7D4) D_803B5F28[buttonIdx].angle = 3.1415927f;
            } else {
                if (D_803B5F28[buttonIdx].angle > 0) D_803B5F28[buttonIdx].angle -= 12.566371f * D_8002EB94;
                if (D_803B5F28[buttonIdx].angle < 0) D_803B5F28[buttonIdx].angle = 0;
                if (D_803BA7D4) D_803B5F28[buttonIdx].angle = 0;
            }
            memcpy(prevPos, D_803B5F28[buttonIdx].position, 12);
            if (D_803B5F28[buttonIdx].fields[0] == 11) D_803B5F28[buttonIdx].position[0] = D_803B64F0 + 0 * D_803B64E8;
            else if (D_803B5F28[buttonIdx].fields[0] == 9) D_803B5F28[buttonIdx].position[0] = D_803B64F0 + -80 * D_803B64E8;
            else D_803B5F28[buttonIdx].position[0] = D_803B64F0 + -120 * D_803B64E8;
            D_803B5F28[buttonIdx].position[1] = buttonY;
            D_803B5F28[buttonIdx].position[2] = D_803B64FC;
            if (!D_803BA7D4) {
                if (fabsf(D_803B5F28[buttonIdx].position[1] - prevPos[1]) * 60 / 10 > D_803BA7DC) {
                    D_803BA7DC = fabsf(D_803B5F28[buttonIdx].position[1] - prevPos[1]) * 60 / 10;
                    D_803BA7DC = D_803BA7DC < 540 * D_803B64E8 ? D_803BA7DC : 540 * D_803B64E8;
                    D_803BA7DC = D_803BA7DC > 180 * D_803B64E8 ? D_803BA7DC : 180 * D_803B64E8;
                }
                if (D_803B5F28[buttonIdx].position[1] > prevPos[1] + D_803BA7DC * D_8002EB94) {
                    D_803B5F28[buttonIdx].position[1] = prevPos[1] + D_803BA7DC * D_8002EB94;
                    clearScrollSpeed = 0;
                } else if (D_803B5F28[buttonIdx].position[1] < prevPos[1] - D_803BA7DC * D_8002EB94) {
                    D_803B5F28[buttonIdx].position[1] = prevPos[1] - D_803BA7DC * D_8002EB94;
                    clearScrollSpeed = 0;
                }
            }
            func_8008B32C(D_8011418C, D_803B5F28[buttonIdx].matrix, D_803B64E8);
            func_800B5898(D_803B5F28[buttonIdx].angle - 1.5707964f, D_803B5F28[buttonIdx].matrix);
            if (D_803B5F28[buttonIdx].angle > 1.5707964f)
                tex = func_800B24EC("BUTTON_SELECT", &texIdx, 0, D_80140BDC - 1, 1);
            else tex = func_800B24EC("BUTTON", &texIdx, 0, D_80140BDC - 1, 1);
            func_8008D870(D_803B5F28[buttonIdx].fields[1], tex, -1);
            if (D_803B5F28[buttonIdx].position[1] > D_803B64F4 + D_803B64EC * D_803B64E8 ||
                D_803B5F28[buttonIdx].position[1] < D_803B64F8 - D_803B64EC * D_803B64E8) D_803B5F28[buttonIdx].alpha = 0;
            else if (D_803B5F28[buttonIdx].position[1] > D_803B64F4)
                D_803B5F28[buttonIdx].alpha = 255.0f - (D_803B5F28[buttonIdx].position[1] - D_803B64F4) * 255.0f / (D_803B64EC * D_803B64E8);
            else if (D_803B5F28[buttonIdx].position[1] < D_803B64F8)
                D_803B5F28[buttonIdx].alpha = 255.0f - (D_803B64F8 - D_803B5F28[buttonIdx].position[1]) * 255.0f / (D_803B64EC * D_803B64E8);
            else D_803B5F28[buttonIdx].alpha = 255;
            if (!D_803BA7F0[buttonIdx]) D_803B5F28[buttonIdx].alpha = 0;
            buttonColor[3] = D_803B5F28[buttonIdx].alpha;
            func_8008E06C(D_803B5F28[buttonIdx].fields[1], buttonColor);
            if (!D_803BA7F0[buttonIdx] || !D_803B5F28[buttonIdx].alpha) model_data_load(D_803B5F28[buttonIdx].fields[1], 1, 15);
            else model_transform_setup(D_803B5F28[buttonIdx].fields[1], 0, 15);
            if (buttonIdx == 1) {
                D_803B5F28[14].position[0] = D_803B64F0 + 160 * D_803B64E8;
                D_803B5F28[14].position[1] = D_803B5F28[buttonIdx].position[1];
                D_803B5F28[14].position[2] = D_803B5F28[buttonIdx].position[2];
                D_803B5F28[14].angle = D_803B5F28[buttonIdx].angle;
                func_8008B32C(D_8011418C, D_803B5F28[14].matrix, D_803B64E8);
                func_800B5898(D_803B5F28[14].angle - 1.5707964f, D_803B5F28[14].matrix);
                func_8008D870(D_803B5F28[14].fields[1], tex, -1);
                D_803B5F28[14].alpha = D_803B5F28[buttonIdx].alpha;
                func_8008E06C(D_803B5F28[14].fields[1], buttonColor);
                if (!D_803BA7F0[buttonIdx] || !D_803B5F28[buttonIdx].alpha) model_data_load(D_803B5F28[14].fields[1], 1, 15);
                else model_transform_setup(D_803B5F28[14].fields[1], 0, 15);
            } else if (D_803BA808 >= 3 && D_803BA808 < 11) {
                if (buttonIdx == D_803BA808) {
                    D_803B5F28[13].position[0] = D_803B64F0 + 120 * D_803B64E8;
                    D_803B5F28[13].position[1] = D_803B5F28[buttonIdx].position[1];
                    D_803B5F28[13].position[2] = D_803B5F28[buttonIdx].position[2];
                    if (D_803BA80C) {
                        if (D_803B5F28[13].angle < 3.1415927f) D_803B5F28[13].angle += 12.566371f * D_8002EB94;
                        if (D_803B5F28[13].angle > 3.1415927f) D_803B5F28[13].angle = 3.1415927f;
                        if (D_803BA7D4) D_803B5F28[13].angle = 3.1415927f;
                    } else {
                        if (D_803B5F28[13].angle > 0) D_803B5F28[13].angle -= 12.566371f * D_8002EB94;
                        if (D_803B5F28[13].angle < 0) D_803B5F28[13].angle = 0;
                        if (D_803BA7D4) D_803B5F28[13].angle = 0;
                    }
                    func_8008B32C(D_8011418C, D_803B5F28[13].matrix, D_803B64E8);
                    func_800B5898(D_803B5F28[13].angle - 1.5707964f, D_803B5F28[13].matrix);
                    if (D_803B5F28[13].angle > 1.5707964f)
                        tex = func_800B24EC("BUTTON_SELECT", &texIdx, 0, D_80140BDC - 1, 1);
                    else tex = func_800B24EC("BUTTON", &texIdx, 0, D_80140BDC - 1, 1);
                    func_8008D870(D_803B5F28[13].fields[1], tex, -1);
                    D_803B5F28[13].alpha = D_803B5F28[buttonIdx].alpha;
                    func_8008E06C(D_803B5F28[13].fields[1], buttonColor);
                    if (!D_803BA7F0[buttonIdx] || !D_803B5F28[13].alpha) model_data_load(D_803B5F28[13].fields[1], 1, 15);
                    else model_transform_setup(D_803B5F28[13].fields[1], 0, 15);
                }
            } else {
                D_803B5F28[13].angle = 0;
                model_data_load(D_803B5F28[13].fields[1], 1, 15);
            }
            if (D_803BA7F0[buttonIdx] == 1) buttonY -= D_803B64EC * D_803B64E8;
        }
        for (; buttonIdx < 13; buttonIdx++) model_data_load(D_803B5F28[buttonIdx].fields[1], 1, 15);
        if (clearScrollSpeed == 1) D_803BA7DC = 0;
        D_803BA7D4 = 0;
    }
}

