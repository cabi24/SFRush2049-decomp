/* func_803923B0 = MAT3 recordscr.c updateButtons (GC 0x80084EE8) with updateViewButtons (GC 0x80085998, 0x8B4 B)
 * inlined by umerge.  updateSelectButtons (GC 0x80084F38) stays the call func_803915E8.
 * Direct typed port of the MAT3 bodies.  N64 differences (from the retail displacements/constants): 23 buttons
 * (D_803B5F28, 0x40 stride), statics gButtonScale..cursorZ at D_803B64E8..D_803B6500 (buttonX at D_803B64F0),
 * alpha fades linearly over one pitch (255 - d * 255 / (space * scale)) instead of a flat 80, and the two trailing
 * MBOX_HideObject(buttonInfo[19/20]) calls are absent.  Callees: MBOX_HideObject = model_data_load,
 * MBOX_ShowObject = model_transform_setup, ZOID_SetTexture = func_8008D870, ZOID_SetColor = func_8008E06C,
 * MBOX_FindTexture_WORK = func_800B24EC, WPitchUV = func_800B5898, WRollUV = func_800B5940, ScaleUV = func_8008B32C.
 */
typedef unsigned short u16;
extern void func_803915E8(void);
extern Color4 D_803B65B0;
extern f32 D_803B64E8, D_803B64EC, D_803B64F0, D_803B64F4;
extern f32 D_803B64F8, D_803B64FC, D_803B6500;
extern f32 D_803BA7D8, D_803BA7DC;
extern volatile f32 D_8002EB94;
extern char D_803B8C60[], D_803B8C70[];
extern f32 fabsf(f32);
#pragma intrinsic(fabsf)

void updateViewButtons(void);

void func_803923B0(void)
{
    s32 pad0, pad1;
    if (D_803BA7D0) {
        switch (D_803BA804) {
        case 0: func_803915E8(); break;
        case 1: updateViewButtons(); break;
        }
    }
}

void updateViewButtons(void)
{
    s32 buttonIdx, i;
    f32 prevPos[3];
    s32 gViewMenuCount;
    s32 whichItem;
    f32 buttonY, selectedPos;
    s8 clearScrollSpeed = 1;
    void *tex;
    u16 texIdx;
    Color4 buttonColor = {255, 255, 255, 255};
    gViewMenuCount = 0;
    whichItem = 0;
    for (i = 0; i < 2; i++) {
        if (D_803BA7FE[i]) {
            gViewMenuCount++;
            if (i < D_803BA810) whichItem++;
        }
    }
    if ((gViewMenuCount - 1) * D_803B64EC * D_803B64E8 > D_803B64F4 - D_803B64F8)
        selectedPos = D_803B64F4 - whichItem * (D_803B64F4 - D_803B64F8) / (gViewMenuCount - 1);
    else selectedPos = D_803B64F4 - whichItem * D_803B64EC * D_803B64E8;
    memcpy(prevPos, D_803B5F28[18].position, 12);
    D_803B5F28[18].angle += 6.2831855f * D_8002EB94;
    if (D_803B5F28[18].angle > 6.2831855f) D_803B5F28[18].angle -= 6.2831855f;
    memcpy(D_803B5F28[18].matrix, D_8011418C, 36);
    D_803B5F28[18].position[0] = D_803B64F0 * D_803B6500 / D_803B64FC + -165.0f * D_803B64E8;
    D_803B5F28[18].position[1] = selectedPos * D_803B6500 / D_803B64FC;
    D_803B5F28[18].position[2] = D_803B6500;
    if (!D_803BA7D4) {
        if (D_803BA7D8 < fabsf(D_803B5F28[18].position[1] - prevPos[1]) * 60.0f * D_803B64E8 / 10.0f) {
            D_803BA7D8 = fabsf(D_803B5F28[18].position[1] - prevPos[1]) * 60.0f * D_803B64E8 / 10.0f;
            D_803BA7D8 = D_803BA7D8 < 540.0f * D_803B64E8 ? D_803BA7D8 : 540.0f * D_803B64E8;
            D_803BA7D8 = D_803BA7D8 > 180.0f * D_803B64E8 ? D_803BA7D8 : 180.0f * D_803B64E8;
        }
        if (D_803B5F28[18].position[1] > prevPos[1] + D_803BA7D8 * D_8002EB94)
            D_803B5F28[18].position[1] = prevPos[1] + D_803BA7D8 * D_8002EB94;
        else if (D_803B5F28[18].position[1] < prevPos[1] - D_803BA7D8 * D_8002EB94)
            D_803B5F28[18].position[1] = prevPos[1] - D_803BA7D8 * D_8002EB94;
        else D_803BA7D8 = 180.0f * D_803B64E8;
    }
    func_800B5940(D_803B5F28[18].angle, D_803B5F28[18].matrix);
    func_800B5898(1.5707964f, D_803B5F28[18].matrix);
    func_8008B32C(D_803B5F28[18].matrix, D_803B5F28[18].matrix, D_803B64E8 * 0.4f);
    buttonY = selectedPos + whichItem * D_803B64EC * D_803B64E8;
    for (buttonIdx = 0; buttonIdx < 11; buttonIdx++) model_data_load(D_803B5F28[buttonIdx].fields[1], 1, 15);
    for (buttonIdx = 11; buttonIdx < 13; buttonIdx++) {
        if (buttonIdx - 11 == D_803BA810) {
            if (D_803B5F28[buttonIdx].angle < 3.1415927f) D_803B5F28[buttonIdx].angle += 12.566371f * D_8002EB94;
            if (D_803B5F28[buttonIdx].angle > 3.1415927f) D_803B5F28[buttonIdx].angle = 3.1415927f;
            if (D_803BA7D4) D_803B5F28[buttonIdx].angle = 3.1415927f;
        } else {
            if (D_803B5F28[buttonIdx].angle > 0.0f) D_803B5F28[buttonIdx].angle -= 12.566371f * D_8002EB94;
            if (D_803B5F28[buttonIdx].angle < 0.0f) D_803B5F28[buttonIdx].angle = 0.0f;
            if (D_803BA7D4) D_803B5F28[buttonIdx].angle = 0.0f;
        }
        memcpy(prevPos, D_803B5F28[buttonIdx].position, 12);
        D_803B5F28[buttonIdx].position[0] = -140.0f * D_803B64E8 + D_803B64F0;
        D_803B5F28[buttonIdx].position[1] = buttonY;
        D_803B5F28[buttonIdx].position[2] = D_803B64FC;
        if (!D_803BA7D4) {
            if (D_803BA7DC < fabsf(D_803B5F28[buttonIdx].position[1] - prevPos[1]) * 60.0f / 10.0f) {
                D_803BA7DC = fabsf(D_803B5F28[buttonIdx].position[1] - prevPos[1]) * 60.0f / 10.0f;
                D_803BA7DC = D_803BA7DC < 540.0f * D_803B64E8 ? D_803BA7DC : 540.0f * D_803B64E8;
                D_803BA7DC = D_803BA7DC > 180.0f * D_803B64E8 ? D_803BA7DC : 180.0f * D_803B64E8;
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
        if (D_803B5F28[buttonIdx].angle > 1.5707964f) tex = func_800B24EC(D_803B8C60, (s16 *)&texIdx, 0, D_80140BDC - 1, 1);
        else tex = func_800B24EC(D_803B8C70, (s16 *)&texIdx, 0, D_80140BDC - 1, 1);
        func_8008D870((s16)D_803B5F28[buttonIdx].fields[1], tex, -1);
        if (D_803B5F28[buttonIdx].position[1] > D_803B64F4 + D_803B64EC * D_803B64E8 || D_803B5F28[buttonIdx].position[1] < D_803B64F8 - D_803B64EC * D_803B64E8) D_803B5F28[buttonIdx].alpha = 0;
        else if (D_803B5F28[buttonIdx].position[1] > D_803B64F4) D_803B5F28[buttonIdx].alpha = 255.0f - (D_803B5F28[buttonIdx].position[1] - D_803B64F4) * 255 / (D_803B64EC * D_803B64E8);
        else if (D_803B5F28[buttonIdx].position[1] < D_803B64F8) D_803B5F28[buttonIdx].alpha = 255.0f - (D_803B64F8 - D_803B5F28[buttonIdx].position[1]) * 255 / (D_803B64EC * D_803B64E8);
        else D_803B5F28[buttonIdx].alpha = 255;
        if (!D_803BA7FE[buttonIdx - 11]) D_803B5F28[buttonIdx].alpha = 0;
        buttonColor.alpha = D_803B5F28[buttonIdx].alpha;
        func_8008E06C((s16)D_803B5F28[buttonIdx].fields[1], &buttonColor);
        if (!D_803BA7FE[buttonIdx - 11] || !D_803B5F28[buttonIdx].alpha) model_data_load(D_803B5F28[buttonIdx].fields[1], 1, 15);
        else model_transform_setup(D_803B5F28[buttonIdx].fields[1], 0, 15);
        if (buttonIdx - 11 == 0) {
            D_803B5F28[15].position[0] = 100.0f * D_803B64E8 + D_803B64F0;
            D_803B5F28[15].position[1] = D_803B5F28[buttonIdx].position[1];
            D_803B5F28[15].position[2] = D_803B5F28[buttonIdx].position[2];
            D_803B5F28[15].angle = D_803B5F28[buttonIdx].angle;
            func_8008B32C(D_8011418C, D_803B5F28[15].matrix, D_803B64E8);
            func_800B5898(D_803B5F28[15].angle - 1.5707964f, D_803B5F28[15].matrix);
            func_8008D870((s16)D_803B5F28[15].fields[1], tex, -1);
            D_803B5F28[15].alpha = D_803B5F28[buttonIdx].alpha;
            func_8008E06C((s16)D_803B5F28[15].fields[1], &buttonColor);
            if (!D_803BA7FE[buttonIdx] || !D_803B5F28[buttonIdx].alpha) model_data_load(D_803B5F28[15].fields[1], 1, 15);
            else model_transform_setup(D_803B5F28[15].fields[1], 0, 15);
        } else if (buttonIdx - 11 == 1) {
            D_803B5F28[16].position[0] = 100.0f * D_803B64E8 + D_803B64F0;
            D_803B5F28[16].position[1] = D_803B5F28[buttonIdx].position[1];
            D_803B5F28[16].position[2] = D_803B5F28[buttonIdx].position[2];
            D_803B5F28[16].angle = D_803B5F28[buttonIdx].angle;
            func_8008B32C(D_8011418C, D_803B5F28[16].matrix, D_803B64E8);
            func_800B5898(D_803B5F28[16].angle - 1.5707964f, D_803B5F28[16].matrix);
            func_8008D870((s16)D_803B5F28[16].fields[1], tex, -1);
            D_803B5F28[16].alpha = D_803B5F28[buttonIdx].alpha;
            func_8008E06C((s16)D_803B5F28[16].fields[1], &buttonColor);
            if (!D_803BA7FE[buttonIdx - 11] || !D_803B5F28[buttonIdx].alpha) model_data_load(D_803B5F28[16].fields[1], 1, 15);
            else model_transform_setup(D_803B5F28[16].fields[1], 0, 15);
        }
        if (D_803BA7FE[buttonIdx - 11] == 1) buttonY -= D_803B64EC * D_803B64E8;
    }
    for (; buttonIdx < 23; buttonIdx++) {
        if (buttonIdx < 15) model_data_load(D_803B5F28[buttonIdx].fields[1], 1, 15);
        else model_transform_setup(D_803B5F28[buttonIdx].fields[1], 0, 15);
    }
    if (clearScrollSpeed == 1) D_803BA7DC = 0.0f;
    D_803BA7D4 = 0;
}
