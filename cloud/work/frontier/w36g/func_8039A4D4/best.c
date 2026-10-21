/* w36g: func_8039A4D4 = updateButtons@circuitscr.c (MAT3 GC 0x8006A730, 0x89C B): per-frame scroll, fade and
 * texture of the four circuit-menu buttons, their right-hand value buttons (4, 5) and the cursor (7, 9). Direct
 * typed port of reference/repos/mat3 decomp/src/GameSource/src/circuitscr.c updateButtons; called only by
 * func_8039C140 (updateCircuitScreen) as its last statement, so it keeps the group's private ABI (saves only ra).
 * N64 names: buttonInfo D_803B30C0, buttonsInited D_803B33C0, buttonsJustInited D_803B33C4, gShowTrophy
 * D_803B342C, gEnterCircuitCode D_803B3414, gConfirmNewCircuit D_803B3418, gCircuitMenuSelect D_803BACF8,
 * gButtonScale D_803B33C8, gButtonSpace D_803B33CC, buttonX D_803B33D0, maxY D_803B33D4, minY D_803B33D8,
 * buttonZ D_803B33DC, cursorZ D_803B33E0, gFrameSeconds D_8002EB94 (volatile), unitmatrix D_8011418C,
 * gNumModels D_80140BDC, buttonColor initialiser D_803B345C, "BUTTON_SELECT"/"BUTTON" D_803B87C8/D_803B87D8;
 * MBOX_ShowObject model_transform_setup, MBOX_HideObject model_data_load, ScaleUV func_8008B32C, WPitchUV
 * func_800B5898, WRollUV func_800B5940, MBOX_FindTexture_WORK func_800B24EC, ZOID_SetTexture func_8008D870,
 * ZOID_SetColor func_8008E06C, menuItemValid func_8039A448.
 * N64 differences read from the retail words: unselected buttons settle to 0.0f (GC 0.08726646f); alpha ramps
 * linearly over one button pitch (GC fixed 80); single-precision speed math (fabsf intrinsic, 60.0f/10.0f).
 * cursorSpeed/scrollSpeed are the function-local statics (retail .bss 0x803BA228/0x803BA22C).
 * Shaping (the carscr sibling's measured idioms, w35a func_8039D6A4): fabsf on the right of the speed tests;
 * the second 255 of each alpha ramp is the int 255 (keeps 255.0f rematerialised, 60.0f hoisted into $f30);
 * gNumModels read through a volatile view (the group's shared declaration stays non-volatile for B560/BE48).
 * FRAME RESIDUAL (disclosed): thirteen unused s32 homes declared first. Retail frame 152 with the locals at
 * sp+60..100 (buttonColor +60, texIdx +66, clearScrollSpeed +75, selectedPos +76, prevPos +84) and 52 bytes of
 * homes above them that no instruction touches; without the homes ugen gives frame 96 (locals +56..96); with
 * them 152 and every slot as retail. Their original names/purpose are unknown (no MAT3/DWARF counterpart; an
 * inlined helper's locals land below the caller's, so it is not an inline). No padding array is used.
 * Own data: six float literals, the two .bss statics. */
void func_8039A4D4(void)
{
    static f32 cursorSpeed;
    static f32 scrollSpeed;
    s32 frame_home0, frame_home1, frame_home2, frame_home3, frame_home4, frame_home5, frame_home6;
    s32 frame_home7, frame_home8, frame_home9, frame_home10, frame_home11, frame_home12;
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
                if (scrollSpeed < fabsf(D_803B30C0[buttonIdx].position[1] - prevPos[1]) * 60.0f / 10.0f) {
                    scrollSpeed = fabsf(D_803B30C0[buttonIdx].position[1] - prevPos[1]) * 60.0f / 10.0f;
                    scrollSpeed = scrollSpeed < 540.0f * D_803B33C8 ? scrollSpeed : 540.0f * D_803B33C8;
                    scrollSpeed = scrollSpeed > 180.0f * D_803B33C8 ? scrollSpeed : 180.0f * D_803B33C8;
                }
                if (D_803B30C0[buttonIdx].position[1] > prevPos[1] + scrollSpeed * D_8002EB94) {
                    D_803B30C0[buttonIdx].position[1] = prevPos[1] + scrollSpeed * D_8002EB94;
                    clearScrollSpeed = 0;
                } else if (D_803B30C0[buttonIdx].position[1] < prevPos[1] - scrollSpeed * D_8002EB94) {
                    D_803B30C0[buttonIdx].position[1] = prevPos[1] - scrollSpeed * D_8002EB94;
                    clearScrollSpeed = 0;
                }
            }
            func_8008B32C((f32 (*)[3])D_8011418C, D_803B30C0[buttonIdx].matrix, D_803B33C8);
            func_800B5898(D_803B30C0[buttonIdx].angle - 1.5707964f, D_803B30C0[buttonIdx].matrix);
            if (D_803B30C0[buttonIdx].angle > 1.5707964f)
                tex = func_800B24EC(D_803B87C8, &texIdx, 0, *(volatile u8 *)&D_80140BDC - 1, 1);
            else
                tex = func_800B24EC(D_803B87D8, &texIdx, 0, *(volatile u8 *)&D_80140BDC - 1, 1);
            func_8008D870(D_803B30C0[buttonIdx].handle, (s32)tex, -1);
            if (D_803B30C0[buttonIdx].position[1] > D_803B33D4 + D_803B33CC * D_803B33C8
                || D_803B30C0[buttonIdx].position[1] < D_803B33D8 - D_803B33CC * D_803B33C8)
                D_803B30C0[buttonIdx].alpha = 0;
            else if (D_803B30C0[buttonIdx].position[1] > D_803B33D4)
                D_803B30C0[buttonIdx].alpha = 255.0f - (D_803B30C0[buttonIdx].position[1] - D_803B33D4) * 255 / (D_803B33CC * D_803B33C8);
            else if (D_803B30C0[buttonIdx].position[1] < D_803B33D8)
                D_803B30C0[buttonIdx].alpha = 255.0f - (D_803B33D8 - D_803B30C0[buttonIdx].position[1]) * 255 / (D_803B33CC * D_803B33C8);
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
        if (cursorSpeed < fabsf(D_803B30C0[7].position[1] - prevPos[1]) * 60.0f / 10.0f) {
            cursorSpeed = fabsf(D_803B30C0[7].position[1] - prevPos[1]) * 60.0f / 10.0f;
            cursorSpeed = cursorSpeed < 540.0f * D_803B33C8 ? cursorSpeed : 540.0f * D_803B33C8;
            cursorSpeed = cursorSpeed > 180.0f * D_803B33C8 ? cursorSpeed : 180.0f * D_803B33C8;
        }
        if (D_803B30C0[7].position[1] > prevPos[1] + cursorSpeed * D_8002EB94)
            D_803B30C0[7].position[1] = prevPos[1] + cursorSpeed * D_8002EB94;
        else if (D_803B30C0[7].position[1] < prevPos[1] - cursorSpeed * D_8002EB94)
            D_803B30C0[7].position[1] = prevPos[1] - cursorSpeed * D_8002EB94;
        else
            cursorSpeed = 180.0f * D_803B33C8;
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
        scrollSpeed = 0.0f;
    D_803B33C4 = 0;
}
