/* updateButtons (MAT3 trackscr.c, GC 0x8008cb54). N64: single-precision speed clamps (60.0f/10.0f),
 * WPitchUV(angle - pi/2) (GC adds pi), and the out-of-range buttons fade over one button space
 * (alpha 255 - d*255/(space*scale)) instead of GC's fixed 80. The two GC function-local statics stay
 * function-local (own .bss: scrollSpeed 0x803BA87C, cursorSpeed 0x803BA85C); as externs uopt does not keep them
 * in registers across the clamp. Literal spellings: two distinct 12.566371 rodata words (0x803B92F4/F8).
 * Frame residual, disclosed (Wave 12/16 rule): retail frame 120; without the three unused f32 locals declared
 * first the frame is 112 and every local sits 4 bytes high (prevPos sp+96 vs retail sp+92). Three 4-byte slots
 * reproduce retail exactly; same 3-slot residual as w35d's startscr updateButtons. Their original use is unknown. */
void func_8038BBB4(void)
{
    static f32 scrollSpeed;
    static f32 cursorSpeed;
    f32 unused0, unused1, unused2;
    s32 buttonIdx;
    f32 prevPos[3];
    f32 buttonY;
    f32 selectedPos;
    s8 clearScrollSpeed = 1;
    Texture *tex;
    s16 texIdx;
    Color buttonColor = D_803B8398;
    if (D_803B7714) {
        if (D_803B771C * ((D_803BA898 - 1) * D_803B7720) > D_803B7728 - D_803B772C)
            selectedPos = D_803B7728 - D_803BA85A * (D_803B7728 - D_803B772C) / (D_803BA898 - 1);
        else selectedPos = D_803B7728 - D_803B771C * (D_803BA85A * D_803B7720);
        memcpy(prevPos, D_803B6B14[43].position, 12);
        D_803B6B14[43].angle += 6.2831854820251465f * D_8002EB94;
        if (D_803B6B14[43].angle > 6.2831854820251465f) D_803B6B14[43].angle -= 6.2831854820251465f;
        D_803B6B14[43].position[0] = D_803B7724 * D_803B7734 / D_803B7730 + -180.0f * D_803B771C;
        D_803B6B14[43].position[1] = selectedPos * D_803B7734 / D_803B7730;
        D_803B6B14[43].position[2] = D_803B7734;
        if (!D_803B7718) {
            if (60.0f * fabsf(D_803B6B14[43].position[1] - prevPos[1]) / 10.0f > cursorSpeed) {
                cursorSpeed = 60.0f * fabsf(D_803B6B14[43].position[1] - prevPos[1]) / 10.0f;
                cursorSpeed = cursorSpeed < 540.0f * D_803B771C ? cursorSpeed : 540.0f * D_803B771C;
                cursorSpeed = cursorSpeed > 180.0f * D_803B771C ? cursorSpeed : 180.0f * D_803B771C;
            }
            if (D_803B6B14[43].position[1] > prevPos[1] + cursorSpeed * D_8002EB94)
                D_803B6B14[43].position[1] = prevPos[1] + cursorSpeed * D_8002EB94;
            else if (D_803B6B14[43].position[1] < prevPos[1] - cursorSpeed * D_8002EB94)
                D_803B6B14[43].position[1] = prevPos[1] - cursorSpeed * D_8002EB94;
            else cursorSpeed = 180.0f * D_803B771C;
        }
        func_8008B32C((f32 (*)[3])D_8011418C, D_803B6B14[43].matrix, D_803B771C * 0.4f);
        func_800B5940(D_803B6B14[43].angle, D_803B6B14[43].matrix);
        func_800B5898(1.5707963705062866f, D_803B6B14[43].matrix);
        buttonY = selectedPos + D_803B771C * (D_803BA85A * D_803B7720);
        for (buttonIdx = 0; buttonIdx < 21; buttonIdx++) {
            if (buttonIdx == D_803BA8B0[D_803BA85A]) {
                if (D_803B6B14[buttonIdx].angle < 3.1415927410125732f) D_803B6B14[buttonIdx].angle += 12.566370964050293f * D_8002EB94;
                if (D_803B6B14[buttonIdx].angle > 3.1415927410125732f) D_803B6B14[buttonIdx].angle = 3.1415927410125732f;
                if (D_803B7718) D_803B6B14[buttonIdx].angle = 3.1415927410125732f;
            } else {
                if (D_803B6B14[buttonIdx].angle > 0.0f) D_803B6B14[buttonIdx].angle -= 12.566371f * D_8002EB94;
                if (D_803B6B14[buttonIdx].angle < 0.0f) D_803B6B14[buttonIdx].angle = 0.0f;
                if (D_803B7718) D_803B6B14[buttonIdx].angle = 0.0f;
            }
            if ((D_8014A110 != 3 || (D_80154628 == 0 && buttonIdx == 20)) && (D_8014A110 != 2 || buttonIdx != 2)) D_803B6B14[buttonIdx+21].angle = D_803B6B14[buttonIdx].angle;
            else D_803B6B14[buttonIdx+21].angle = 0.0f;
            memcpy(prevPos, D_803B6B14[buttonIdx].position, 12);
            D_803B6B14[buttonIdx].position[0] = D_803B7724 + -155.0f * D_803B771C;
            D_803B6B14[buttonIdx].position[1] = buttonY;
            D_803B6B14[buttonIdx].position[2] = D_803B7730;
            if (!D_803B7718) {
                if (60.0f * fabsf(D_803B6B14[buttonIdx].position[1] - prevPos[1]) / 10.0f > scrollSpeed) {
                    scrollSpeed = 60.0f * fabsf(D_803B6B14[buttonIdx].position[1] - prevPos[1]) / 10.0f;
                    scrollSpeed = scrollSpeed < 540.0f * D_803B771C ? scrollSpeed : 540.0f * D_803B771C;
                    scrollSpeed = scrollSpeed > 180.0f * D_803B771C ? scrollSpeed : 180.0f * D_803B771C;
                }
                if (D_803B6B14[buttonIdx].position[1] > prevPos[1] + scrollSpeed * D_8002EB94) {
                    D_803B6B14[buttonIdx].position[1] = prevPos[1] + scrollSpeed * D_8002EB94;
                    clearScrollSpeed = 0;
                } else if (D_803B6B14[buttonIdx].position[1] < prevPos[1] - scrollSpeed * D_8002EB94) {
                    D_803B6B14[buttonIdx].position[1] = prevPos[1] - scrollSpeed * D_8002EB94;
                    clearScrollSpeed = 0;
                }
            }
            D_803B6B14[buttonIdx+21].position[0] = D_803B7724 + 50.0f * D_803B771C;
            D_803B6B14[buttonIdx+21].position[1] = D_803B6B14[buttonIdx].position[1];
            D_803B6B14[buttonIdx+21].position[2] = D_803B6B14[buttonIdx].position[2];
            func_8008B32C((f32 (*)[3])D_8011418C, D_803B6B14[buttonIdx].matrix, D_803B771C);
            func_8008B32C((f32 (*)[3])D_8011418C, D_803B6B14[buttonIdx+21].matrix, D_803B771C);
            func_800B5898(D_803B6B14[buttonIdx].angle - 1.5707963705062866f, D_803B6B14[buttonIdx].matrix);
            func_800B5898(D_803B6B14[buttonIdx+21].angle - 1.5707963705062866f, D_803B6B14[buttonIdx+21].matrix);
            if (D_803B6B14[buttonIdx].angle > 1.5707963705062866f) tex = func_800B24EC(D_803B9284, &texIdx, 0, D_80140BDC - 1, 1);
            else tex = func_800B24EC(D_803B9294, &texIdx, 0, D_80140BDC - 1, 1);
            func_8008D870(D_803B6B14[buttonIdx].handle, (s32)tex, -1);
            if (D_803B6B14[buttonIdx+21].angle > 1.5707963705062866f) tex = func_800B24EC(D_803B929C, &texIdx, 0, D_80140BDC - 1, 1);
            else tex = func_800B24EC(D_803B92AC, &texIdx, 0, D_80140BDC - 1, 1);
            func_8008D870(D_803B6B14[buttonIdx+21].handle, (s32)tex, -1);
            if (D_803B6B14[buttonIdx].position[1] > D_803B7728 + D_803B7720 * D_803B771C || D_803B6B14[buttonIdx].position[1] < D_803B772C - D_803B7720 * D_803B771C) D_803B6B14[buttonIdx].alpha = 0;
            else if (D_803B6B14[buttonIdx].position[1] > D_803B7728) D_803B6B14[buttonIdx].alpha = 255 - (D_803B6B14[buttonIdx].position[1] - D_803B7728) * 255.0f / (D_803B7720 * D_803B771C);
            else if (D_803B6B14[buttonIdx].position[1] < D_803B772C) D_803B6B14[buttonIdx].alpha = 255 - (D_803B772C - D_803B6B14[buttonIdx].position[1]) * 255.0f / (D_803B7720 * D_803B771C);
            else D_803B6B14[buttonIdx].alpha = 255;
            if (!func_8038ACB0(buttonIdx)) D_803B6B14[buttonIdx].alpha = 0;
            D_803B6B14[buttonIdx+21].alpha = D_803B6B14[buttonIdx].alpha;
            buttonColor.channel[3] = D_803B6B14[buttonIdx].alpha;
            func_8008E06C(D_803B6B14[buttonIdx].handle, (s32 *)&buttonColor);
            func_8008E06C(D_803B6B14[buttonIdx+21].handle, (s32 *)&buttonColor);
            if (!func_8038ACB0(buttonIdx)) {
                model_data_load(D_803B6B14[buttonIdx].handle, 0, 15);
                model_data_load(D_803B6B14[buttonIdx+21].handle, 0, 15);
            } else if (D_803B6B14[buttonIdx].alpha == 0) {
                model_data_load(D_803B6B14[buttonIdx].handle, 0, 15);
                model_data_load(D_803B6B14[buttonIdx+21].handle, 0, 15);
                buttonY -= D_803B7720 * D_803B771C;
            } else {
                model_transform_setup(D_803B6B14[buttonIdx].handle, 0, 15);
                model_transform_setup(D_803B6B14[buttonIdx+21].handle, 0, 15);
                buttonY -= D_803B7720 * D_803B771C;
            }
        }
        if (clearScrollSpeed == 1) scrollSpeed = 0.0f;
        D_803B7718 = 0;
    }
}
