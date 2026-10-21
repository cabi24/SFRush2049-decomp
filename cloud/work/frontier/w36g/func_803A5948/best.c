/* w36g: func_803A5948 = animText@circuitscr.c (MAT3 GC 0x800695FC), with animButtonText@circuitscr.c
 * (GC 0x80068850) and getFinishPlaceStr@circuitscr.c (GC 0x8006A000) inlined into it by umerge; their
 * deleted bodies are the caller-less `jr ra; nop` stubs func_803A5940 and func_803A5938 (non-static, off
 * the keep list, so each stub keeps its symbol). Direct typed port of reference/repos/mat3
 * decomp/src/GameSource/src/circuitscr.c animButtonText/animText/getFinishPlaceStr; N64 names below:
 * buttonInfo D_803B30C0 (tMenuButton[12], 64 B), buttonsInited D_803B33C0, gButtonScale D_803B33C8,
 * gEnterCircuitCode D_803B3414, gConfirmNewCircuit D_803B3418, gInvalidCode D_803B341C,
 * gWrongNumPlayers D_803B3420, gContinueEnabled D_803B3424, gStatsEnabled D_803B3428, gShowTrophy
 * D_803B342C, gCircuitMenuSelect D_803BACF8, newCircuitType D_803BAD38, gCircuit D_803BAD90 (b8 type,
 * b9 track), gCircuitCode D_803BAD98, gCodeSelect D_803B9A80, gReviewTrack D_803B9A84, gYes D_803BADC4,
 * pane D_803BACF0, gPasswordAlphabet D_80116FE4, gNumCircuitTracks D_80111784, gCircuitCars D_80154450,
 * flashColors D_801146BC/C0, gCamera D_80150B70, gText D_8017A4E0 (single +4, tab +0xC, taba +0x10);
 * HAL_SetTextDepth render_helper, TB_SetJust func_800B669C, ZOID_ProjectWindow brake_light_update,
 * HAL_SetFont object_create, TB_SetTextColor dispatch_handler, TB_CenterText sound_pitch_diff_halved,
 * TB_DrawText state_utility, TB_DrawTextScrollBox camera_auto_follow, TB_GetFontLineHeight
 * object_bytes_sum_global, HAL_SetTextTranslucency func_800ED66C, TB_FlashTextColors func_800BEA3C (tRGBA
 * by value on N64), TB_Sprintf fcvt_wrapper, TB_Strcpy func_800BE6A4, TB_Strcat func_800BE4F0,
 * HAL_GetTextWidth object_manager_update (MAT3 returns unsigned int: the macro below restores that type
 * for these calls without touching the shared s32 declaration the old members use), ChangePaneSize
 * crowd_cheer_play, getPointsStr func_8039B908, getPlaceStr func_8039B988, getToGoStr func_8039BA28,
 * getFinishedStr func_8039BAE0, drawReview func_803A5658.
 * N64 differences from the GC body, all read from the retail words: text indices and layout constants
 * (59/75/126/129/153, pane boxes), screenPos used unhalved, the 5-track cap when D_80156994 == 0, a
 * code-entry loop that steps x before the row test, and the del static at D_803B3454 (its "DEL" literal
 * is the image's D_803B8758, written as that symbol like the group's other literals). Shaping, measured:
 * - Local order i, str1, str, screenPos, x, y and a 192-byte str1 (GC: i, x, y, str1[64], str, screenPos)
 *   are the only layout that reproduces the retail frame (str1 +140, str +136, screenPos +132, x/y homes
 *   below; 344 bytes). No unused locals.
 * - getFinishPlaceStr is defined after animText (MAT3 file order); defined before it, umerge emits its
 *   parameter copy one instruction early.
 * - The two button-label strings go through `str` before TB_DrawText (as1 order of the argument loads).
 * - del is a block static as in MAT3: as an extern, uopt keeps &D_803B3454 in s0 across TB_CenterText.
 * Own data: three float literals (bits checked against the image at 0x803B9814..1C) and the del word
 * (0x803B3454 = 0x803B8758 in the image). */
extern u8 D_803B8758[];
#define HAL_GetTextWidth(s, n) ((u32)object_manager_update((s), (n)))
void func_803A5938(s32 p, u8 *str);

void func_803A5940(void)
{
    s32 i;
    u8 str1[192];
    u8 *str;
    s16 screenPos[2];
    s32 x;
    s32 y;

    render_helper(0.0f);
    func_800B669C(0, 1);
    brake_light_update(0, D_803B30C0[6].position, &D_80150B70[0], 0, screenPos);
    x = screenPos[0];
    y = screenPos[1];
    object_create(13);
    dispatch_handler(1);
    str = D_8017A4E0.text[42];
    state_utility(sound_pitch_diff_halved(str, x), y, str);
    if (D_803B3414) {
        static u8 *del = D_803B8758;
        func_800B669C(0, 3);
        object_create(11);
        y = 59;
        x = 115;
        dispatch_handler(1);
        state_utility(sound_pitch_diff_halved(D_8017A4E0.text[44], 160), y, D_8017A4E0.text[44]);
        y += 16;
        if (D_803B341C || D_803B3420) {
            dispatch_handler(5);
            state_utility(sound_pitch_diff_halved(D_8017A4E0.text[134], 160), y, D_8017A4E0.text[134]);
        }
        y += 16;
        dispatch_handler(12);
        str1[1] = 0;
        for (i = 0; i < 20; i++) {
            str1[0] = D_803BAD98[i];
            state_utility(sound_pitch_diff_halved(str1, x + 4), y, str1);
            if ((i + 1) % 10 == 0) {
                x = 115;
                y += 9;
            } else
                x += 9;
        }
        dispatch_handler(1);
        x = 94;
        y = 126;
        for (i = 0; i < 32; i++) {
            str1[0] = D_80116FE4[i];
            dispatch_handler(D_803B9A80 == i ? 22 : 1);
            state_utility(sound_pitch_diff_halved(str1, x + 5), y, str1);
            if ((i + 1) % 12 == 0) {
                x = 94;
                y += 11;
            } else
                x += 11;
        }
        x += 12;
        dispatch_handler(D_803B9A80 == -2 ? 22 : 1);
        state_utility(sound_pitch_diff_halved(del, x + 5), y, del);
        if (D_803BACF0)
            crowd_cheer_play(D_803BACF0, 80, 55, 240, 165);
    } else if (D_803B3418) {
        func_800B669C(1, 3);
        object_create(11);
        dispatch_handler(1);
        camera_auto_follow(160, 59, 140, 78, -1, 0, D_8017A4E0.text[96]);
        y = 59 + 5 * object_bytes_sum_global();
        dispatch_handler(D_803BADC4 ? 22 : 1);
        state_utility(145, y, D_8017A4E0.names[((u16 *)D_8017A4E0.indices)[24]]);
        dispatch_handler(D_803BADC4 ? 1 : 22);
        state_utility(175, y, D_8017A4E0.names[((u16 *)D_8017A4E0.indices)[24] + 1]);
        func_800B669C(0, 3);
        if (D_803BACF0)
            crowd_cheer_play(D_803BACF0, 60, 55, 260, 130);
    } else {
        object_create(11);
        for (i = 0; i < 4; i++) {
            if (D_803B30C0[i].angle > 0.6981317f && D_803B30C0[i].angle < 2.443461f)
                continue;
            if (!D_803B30C0[i].alpha)
                continue;
            func_800ED66C(D_803B30C0[i].alpha);
            brake_light_update(0, D_803B30C0[i].position, &D_80150B70[0], 0, screenPos);
            x = screenPos[0] - 35.0f * D_803B33C8;
            y = screenPos[1];
            if (D_803B30C0[i].angle < 1.5707964f) {
                func_800BEA3C(D_801146BC, D_801146C0);
                dispatch_handler(1);
            } else {
                func_800BEA3C(D_801146BC, D_801146C0);
                dispatch_handler(22);
            }
            str = D_8017A4E0.names[((u16 *)D_8017A4E0.indices)[43] + i];
            state_utility(x, y, str);
            if (i == 1) {
                brake_light_update(0, D_803B30C0[4].position, &D_80150B70[0], 0, screenPos);
                x = screenPos[0] - 35.0f * D_803B33C8;
                y = screenPos[1];
                if (D_803B9A84 == D_803BAD90->b9)
                    str = D_8017A4E0.text[176];
                else {
                    fcvt_wrapper(str1, D_803B875C, D_8017A4E0.text[82], D_803B9A84 + 1);
                    str = str1;
                }
                state_utility(x, y, str);
            }
            if (i == 2) {
                brake_light_update(0, D_803B30C0[5].position, &D_80150B70[0], 0, screenPos);
                x = screenPos[0] - 35.0f * D_803B33C8;
                y = screenPos[1];
                str = D_8017A4E0.names[((u16 *)D_8017A4E0.indices)[44] + D_803BAD38];
                state_utility(x, y, str);
            }
        }
        func_800ED66C(-1.0f);
        if (D_803B342C) {
            s16 width;
            str = str1;
            func_800B669C(0, 3);
            object_create(11);
            dispatch_handler(1);
            fcvt_wrapper(str, D_803B8764, D_8017A4E0.names[((u16 *)D_8017A4E0.indices)[44] + D_803BAD90->b8], D_8017A4E0.text[76]);
            width = HAL_GetTextWidth(str, -1) < 20 ? 20 : HAL_GetTextWidth(str, -1);
            state_utility(sound_pitch_diff_halved(str, 160), 153, str);
            y = 153 + object_bytes_sum_global();
            y += object_bytes_sum_global();
            func_803A5938(0, str);
            width = width > HAL_GetTextWidth(str, -1) ? width : HAL_GetTextWidth(str, -1);
            state_utility(sound_pitch_diff_halved(str, 160), y, str);
            y += object_bytes_sum_global();
            func_8039B908(0, str);
            width = width > HAL_GetTextWidth(str, -1) ? width : HAL_GetTextWidth(str, -1);
            state_utility(sound_pitch_diff_halved(str, 160), y, str);
            y += object_bytes_sum_global();
            crowd_cheer_play(D_803BACF0, 160 - width / 2 - 4, 149, 160 + width / 2 + 4, y + 4);
        } else if (D_803BACF8 == 2) {
            s32 numCircuitTracks;
            s16 width;
            str = str1;
            func_800B669C(0, 3);
            object_create(11);
            dispatch_handler(1);
            fcvt_wrapper(str, D_803B876C, D_8017A4E0.names[((u16 *)D_8017A4E0.indices)[44] + D_803BAD38], D_8017A4E0.text[42]);
            width = HAL_GetTextWidth(str, -1) < 20 ? 20 : HAL_GetTextWidth(str, -1);
            state_utility(sound_pitch_diff_halved(str, 160), 129, str);
            y = 129 + object_bytes_sum_global();
            numCircuitTracks = D_80111784[D_803BAD38];
            if (!D_80156994 && numCircuitTracks >= 5)
                numCircuitTracks = 5;
            fcvt_wrapper(str, D_803B8774, numCircuitTracks * (D_803BAD38 == 3 ? 4 : 2), D_8017A4E0.text[71]);
            width = width > HAL_GetTextWidth(str, -1) ? width : HAL_GetTextWidth(str, -1);
            state_utility(sound_pitch_diff_halved(str, 160), y, str);
            y += object_bytes_sum_global();
            crowd_cheer_play(D_803BACF0, 160 - width / 2 - 4, 125, 160 + width / 2 + 4, y + 4);
        } else if (D_803B3428) {
            str = str1;
            func_800B669C(0, 3);
            object_create(11);
            dispatch_handler(1);
            if (D_803BACF8 != 1) {
                s16 width;
                if (D_803B3424) {
                    fcvt_wrapper(str, D_803B877C, D_8017A4E0.names[((u16 *)D_8017A4E0.indices)[44] + D_803BAD90->b8], D_8017A4E0.text[67]);
                    width = HAL_GetTextWidth(str, -1) < 20 ? 20 : HAL_GetTextWidth(str, -1);
                    state_utility(sound_pitch_diff_halved(str, 160), 129, str);
                    y = 129 + object_bytes_sum_global();
                    func_8039BAE0(str);
                    width = width > HAL_GetTextWidth(str, -1) ? width : HAL_GetTextWidth(str, -1);
                    state_utility(sound_pitch_diff_halved(str, 160), y, str);
                    y += object_bytes_sum_global();
                    func_8039BA28(str);
                    width = width > HAL_GetTextWidth(str, -1) ? width : HAL_GetTextWidth(str, -1);
                    state_utility(sound_pitch_diff_halved(str, 160), y, str);
                    y += object_bytes_sum_global();
                } else {
                    fcvt_wrapper(str, D_803B8784, D_8017A4E0.names[((u16 *)D_8017A4E0.indices)[44] + D_803BAD90->b8], D_8017A4E0.text[76]);
                    width = HAL_GetTextWidth(str, -1) < 20 ? 20 : HAL_GetTextWidth(str, -1);
                    state_utility(sound_pitch_diff_halved(str, 160), 129, str);
                    y = 129 + object_bytes_sum_global();
                }
                y += object_bytes_sum_global();
                func_8039B988(0, str);
                width = width > HAL_GetTextWidth(str, -1) ? width : HAL_GetTextWidth(str, -1);
                state_utility(sound_pitch_diff_halved(str, 160), y, str);
                y += object_bytes_sum_global();
                func_8039B908(0, str);
                width = width > HAL_GetTextWidth(str, -1) ? width : HAL_GetTextWidth(str, -1);
                state_utility(sound_pitch_diff_halved(str, 160), y, str);
                y += object_bytes_sum_global();
                crowd_cheer_play(D_803BACF0, 160 - width / 2 - 4, 125, 160 + width / 2 + 4, y + 4);
            } else {
                func_800B669C(0, 3);
                func_803A5658();
                if (D_803BACF0)
                    crowd_cheer_play(D_803BACF0, 12, 125, 308, 205);
            }
        } else
            crowd_cheer_play(D_803BACF0, -20, 100, -10, 120);
    }
    func_800B669C(0, 3);
    render_helper(-1.0f);
}

s32 func_803A5948(Blit *blit)
{
    if (D_803B33C0 == 1) {
        func_803A5940();
        return 1;
    }
    return 1;
}

void func_803A5938(s32 p, u8 *str)
{
    func_800BE6A4(str, D_8017A4E0.text[78]);
    func_800BE4F0(str, D_8017A4E0.names[D_8017A4E0.indices->name_base + D_80154450[p].rank]);
    func_800BE4F0(str, D_803B87A4);
    func_800BE4F0(str, D_8017A4E0.text[129]);
}

#undef HAL_GetTextWidth
