/* updateTrackScreen (MAT3 trackscr.c, GC 0x80087abc). N64 differences read from the retail words: the
 * _InitScreen/showTrackSelect calls come before the player loop, which tests a removed controller pak
 * instead of walking gPlayerFiles; A/B start needs D_80156994 or a track other than 5 (the same gate guards
 * the up/down option moves); effect ids 38/40/39; case 0 sets buttonsJustInited when !D_80156994;
 * options are saved for player 0 after SetOptions; ZeroCamera/UpdateCameras = inlined setCamera. */
void func_8038E5DC(void)
{
    s32 i;
    s32 j;
    s32 add;
    if (D_801174BC != 1) {
        if (D_801174BC != D_801174B4) { func_800B5570(0x100); return; }
        D_801174BC = 1;
    }
    if (!D_803B7CD4) func_8038DFEC();
    func_8038C5EC();
    for (i = 0; i < D_8014A108; i++) {
        if ((*D_8014A118[i].save)->file && func_800A361C((*(*D_8014A118[i].save)->file)->controller)) {
            func_8038B940(); func_800B5570(0x20); return;
        }
    }
    if (D_80157244) { func_8038B940(); func_800B5570(4); return; }
    if ((D_8015694C & 3) && (D_80156994 || D_8014978C != 5)) {
        resource_type_select(D_8015694C); func_800B5570(0x100); func_8038B940();
        for (j = 0; j < D_8014A108; j++)
            for (i = 21; i < 41; i++) audio_bus_mix(D_8014A118[j].save, i, D_80146108[i]);
        return;
    }
    if (D_8015694C & 4) {
        entity_flags_apply(38, 0, 1, 0); func_8038B940();
        if (D_8014A110 == 3) func_800B5570(0x4000);
        else func_800B5570(0x40);
        return;
    }
    if (D_80156944 & 0x3080) {
        add = (D_80149784 & 0x1000) ? -1 : (D_80149784 & 0x2000) ? 1 : 0;
        if ((D_8014A110 == 3 && (D_80154628 > 0 || D_803BA8B0[D_803BA85A] != 20)) || (add == 0 && !D_803BA910) || (D_8014A110 == 2 && D_803BA8B0[D_803BA85A] == 2)) goto controls_done;
        audio_doppler(D_80149784);
        if (D_8013F1D9 > 1) add = -add;
        switch (D_803BA8B0[D_803BA85A]) {
        case 0:
            if (add) {
                D_803BA856 += add;
                if (D_803BA856 < 0) D_803BA856 = D_803BA852 - 1;
                else if (D_803BA856 >= D_803BA852) D_803BA856 = 0;
                do {
                    D_8014978C += add;
                    if (D_8014978C < 0) D_8014978C = 18;
                    else if (D_8014978C >= 19) D_8014978C = 0;
                } while (!func_8038B834(D_8014978C));
                for (i = 21; i < 41; i++)
                    D_80146108[i] = reverb_setup(D_8014A118[0].save, i);
                if (!D_80156994) D_803B7718 = 1;
            }
            break;
        case 1:
            if (add) {
                D_80146108[25] += add;
                if (D_80146108[25] < 0) D_80146108[25] = 1;
                else if (D_80146108[25] > 1) D_80146108[25] = 0;
            } else D_80146108[25] = 0;
            break;
        case 2:
            if (add) {
                D_80146108[21] += add;
                if (D_80146108[21] < 1) D_80146108[21] = 8;
                else if (D_80146108[21] > 8) D_80146108[21] = 1;
            } else D_80146108[21] = 3;
            break;
        case 3:
            if (add) {
                D_80146108[32] += add;
                if (D_80146108[32] < 0) D_80146108[32] = 2;
                else if (D_80146108[32] > 2) D_80146108[32] = 0;
            } else D_80146108[32] = 0;
            break;
        case 4:
            if (add) {
                if (!D_80146108[33]) D_80146108[33] = 1;
                else D_80146108[33] = 0;
            } else D_80146108[33] = 0;
            break;
        case 5:
            if (add) {
                D_80146108[34] += add;
                if (D_80146108[34] <= 0) D_80146108[34] = 100;
                else if (D_80146108[34] > 100) D_80146108[34] = 1;
            } else D_80146108[34] = 69;
            break;
        case 6:
            if (add) {
                D_80146108[35] += add;
                if (D_80146108[35] <= 0) D_80146108[35] = 20;
                else if (D_80146108[35] > 20) D_80146108[35] = 1;
            } else D_80146108[35] = 5;
            break;
        case 7:
            if (add) {
                D_80146108[36] += add;
                if (D_80146108[36] <= 0) D_80146108[36] = 50;
                else if (D_80146108[36] > 50) D_80146108[36] = 1;
            } else D_80146108[36] = 30;
            break;
        case 8:
            if (add) {
                if (!D_80146108[37]) D_80146108[37] = 1;
                else D_80146108[37] = 0;
            } else D_80146108[37] = 0;
            break;
        case 9:
            if (add) {
                D_80146108[38] += 5*add;
                if (D_80146108[38] < 5) D_80146108[38] = 50;
                else if (D_80146108[38] > 50) D_80146108[38] = 5;
            } else D_80146108[38] = 10;
            break;
        case 10:
            if (add) {
                D_80146108[39] += add;
                if (D_80146108[39] <= 0) D_80146108[39] = 20;
                else if (D_80146108[39] > 20) D_80146108[39] = 1;
            } else D_80146108[39] = 8;
            break;
        case 11:
            if (add) {
                D_80146108[40] ^= 1;
            } else D_80146108[40] = 0;
            break;
        case 12:
            if (add) {
                D_80146108[27] ^= 1;
            } else D_80146108[27] = 0;
            func_8038B388(); func_8038AEB0(); D_803B7CE0 = 1.0f - D_803B7CE0;
            break;
        case 13:
            if (add) {
                if ((D_8014A118[0].levels & 0x3c000) == 0x3c000) D_80146108[28] = 2;
                else if (!D_80146108[28]) D_80146108[28] = 1;
                else D_80146108[28] = 0;
            } else D_80146108[28] = 0;
            func_8038B388(); func_8038AEB0();
            break;
        case 14:
            if (add) {
                D_80146108[29] += add;
                if (D_80146108[29] < 0) D_80146108[29] = 3;
                else if (D_80146108[29] > 3) D_80146108[29] = 0;
            } else D_80146108[29] = 0;
            break;
        case 15:
            if (add) {
                D_80146108[30] += add;
                if (D_80146108[30] < 0) D_80146108[30] = 4;
                else if (D_80146108[30] > 4) D_80146108[30] = 0;
            } else D_80146108[30] = 0;
            break;
        case 16:
            if (add) {
                D_80146108[23] += add;
                if (D_80146108[23] < 0) D_80146108[23] = 3;
                else if (D_80146108[23] > 3) D_80146108[23] = 0;
            } else D_80146108[23] = 1;
            break;
        case 17:
            if (add) {
                D_80146108[22] += add;
                if (D_80146108[22] > 5) D_80146108[22] = 0;
                else if (D_80146108[22] < 0) D_80146108[22] = 5;
            } else D_80146108[22] = 5;
            break;
        case 18:
            if (add) {
                D_80146108[24] += add;
                if (D_80146108[24] < 0) D_80146108[24] = 5;
                else if (D_80146108[24] > 5) D_80146108[24] = 0;
            } else D_80146108[24] = 2;
            break;
        case 19:
            if (add) {
                D_80146108[26] += add;
                if (D_80146108[26] < 0) D_80146108[26] = 2;
                else if (D_80146108[26] > 2) D_80146108[26] = 0;
            } else D_80146108[26] = 2;
            break;
        case 20:
            if (add) {
                D_80146108[31] ^= 1;
            } else D_80146108[31] = 0;
            if (D_8014A110 == 3) (*(*D_8014A118[D_801543D4].save)->stats)[1785] = D_80146108[31];
            break;
        }
        init_state_begin();
        for (i = 21; i < 41; i++) audio_bus_mix(D_8014A118[0].save, i, D_80146108[i]);
    } else if (D_80149784 & 0x400) {
        if (D_80156994 || D_8014978C != 5) {
            entity_flags_apply(40, 0, 1, 0);
            if (--D_803BA85A < 0) D_803BA85A = D_803BA898 - 1;
        }
    } else if (D_80149784 & 0x800) {
        if (D_80156994 || D_8014978C != 5) {
            entity_flags_apply(39, 0, 1, 0);
            D_803BA85A++;
            if (D_803BA85A > D_803BA898 - 1) D_803BA85A = 0;
        }
    }
    if (D_803BA878 > D_803BA85A - 1) D_803BA878 = D_803BA85A - 1;
    if (D_803BA878 < D_803BA85A - 2) D_803BA878 = D_803BA85A - 2;
    if (D_803BA878 > D_803BA898 - 4) D_803BA878 = D_803BA898 - 4;
    if (D_803BA878 < 0) D_803BA878 = 0;
controls_done:
    D_803BA908 = D_8014A110 != 3 || (D_80154628 == 0 && D_803BA8B0[D_803BA85A] == 20);
    if (D_8014978C != D_803BA84E) {
        D_803BAC90 = D_8014978C;
        D_803BA84E = D_8014978C;
        for (i = 0; i < D_8014A108; i++) {
            switch (D_8014A110) {
            case 0: case 1: case 2:
                if (D_8014978C >= 0 && D_8014978C <= 5) menu_text_input(D_8014A118[i].save, D_8014A110, D_8014978C + 0);
                break;
            case 4:
                if (D_8014978C >= 14 && D_8014978C <= 17) menu_text_input(D_8014A118[i].save, D_8014A110, D_8014978C - 14);
                break;
            case 6:
                if (D_8014978C >= 6 && D_8014978C <= 13) menu_text_input(D_8014A118[i].save, D_8014A110, D_8014978C - 6);
                break;
            }
        }
    }
    func_8038B3EC();
    func_8038A820();
    func_8038A634(0);
    func_8038A62C();
}
