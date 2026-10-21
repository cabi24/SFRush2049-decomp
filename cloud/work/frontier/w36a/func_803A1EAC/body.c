void func_803A1EAC(void)
{
    s16 i;
    s16 p;
    s32 sound = 1;
    Record **ghosth;
    Record *ghost;
    Record **nextGhost;

    if (!D_80156994) D_803BA010 = D_8012E6F8;
    else D_803BA010 = D_8012E6F8 + ((D_80152570 ? 6 : 0) + D_8014978C) * 3;
    if (D_801174BC != 1) {
        if (D_801174BC != D_801174B4) { func_803A0498(); func_800B5570(0x40000); return; }
        D_801174BC = 1;
    }
    for (i = 1; i < 4; i++) {
        for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
            ghost = *ghosth;
            nextGhost = ghost->next;
            if (ghost->file && !func_800A1A3C((*ghost->file)->controller)) continue;
            if (ghosth == D_80152698[i]) break;
        }
        if (!ghosth) {
            for (p = 0; p < 3; p++) {
                ghosth = D_803BA010 + p;
                if (ghosth == D_80152698[i]) break;
                ghosth = 0;
            }
        }
        D_80152698[i] = ghosth;
    }
    if (!D_803B28A8) func_803A0FD8();
    func_8039EF08();
    for (i = 0; i < D_8014A108; i++) {
        if ((*D_8014A118[i].owner)->file && func_800A361C((*(*D_8014A118[i].owner)->file)->controller)) {
            func_8039D05C(); func_800B5570(32); D_803BA030 = 0; return;
        }
    }
    if (D_80157244) { func_8039D05C(); func_800B5570(4); D_803BA030 = 0; return; }
    D_803BA030++;
    for (p = 0; p < D_8014A108; p++) {
        s32 notReadyCount;
        s32 notReadyIdx;
        s32 pIdx;
        s16 x, y, w, h;
        s16 screenPos[2];
        Vec3 msgPos;
        s32 ghostIdx;
        s16 gx, gy, gw, gh;
        f32 pos[3];
        s16 gscreenPos[2];
        if (!D_803BA02C && (D_8014A118[p].pressed & 4)) {
            if (D_803BA028[p] == 1) D_803BA028[p] = 0;
            else { entity_flags_apply(38, 0, 1, 0); func_8039D05C(); func_800B5570(128); D_803BA030 = 0; return; }
        }
        if (D_803BA028[p]) {
            msgPos = D_803B3030;
            if (D_80151AD0 == 1) object_create(13); else object_create(12);
            brake_light_update(p, msgPos.v, &D_80150B70[p], 0, screenPos);
            x = screenPos[0];
            y = screenPos[1] - object_bytes_sum_global();
            notReadyCount = 0;
            for (pIdx = 0; pIdx < D_8014A108; pIdx++) {
                if (pIdx != p && !D_803BA028[pIdx]) { notReadyIdx = pIdx; notReadyCount++; }
            }
            if (!notReadyCount) {
                y += object_bytes_sum_global();
                h = object_bytes_sum_global();
                w = object_manager_update(D_803B857C, -1);
            } else {
                w = object_manager_update(countdown_state.text[151], -1);
                h = object_bytes_sum_global() * 2;
                if (notReadyCount == 1) w = (w > object_manager_update((*D_8014A118[notReadyIdx].owner)->name, -1) ? w : object_manager_update((*D_8014A118[notReadyIdx].owner)->name, -1));
                else w = (w > object_manager_update(countdown_state.text[31], -1) ? w : object_manager_update(countdown_state.text[31], -1));
            }
            y -= object_bytes_sum_global() / 2;
            crowd_cheer_play(D_803B9FC0[p], x - w / 2 - 4, y - 4, x + w / 2 + 4, y + h + 4);
            func_8039D6A4(p);
            continue;
        }
        crowd_cheer_play(D_803B9FC0[p], -20, 0, -10, 10);
        if (D_8014A118[p].pressed & 3) {
            resource_type_select(D_8014A118[p].pressed);
            D_803BA028[p] = 1;
            D_80153E88[p].flags = 160;
            D_80153E88[p].car = D_803B9FD0[p];
            if (D_8014A110 == 6 && D_801164BE == 0) {
                D_80153E88[p].color[0] = D_80138664[D_8012E67C[p]];
                D_80153E88[p].color[1] = D_80138664[D_8012E67C[p]];
                D_80153E88[p].color[2] = D_80138664[D_8012E67C[p]];
            } else {
                D_80153E88[p].color[0] = D_80111604[p + 1][D_803B9FD0[p]];
                D_80153E88[p].color[1] = D_80111648[p + 1][D_803B9FD0[p]];
                D_80153E88[p].color[2] = D_8011168C[p + 1][D_803B9FD0[p]];
            }
            D_803BA030 = 0;
        }
        if (D_8014A118[p].repeated & 0x400) {
            entity_flags_apply(40, 0, 1, 0);
            D_803B28B8[p]--;
            if (D_803B28B8[p] < 0) D_803B28B8[p] = D_803BA050[p] - 1;
        } else if (D_8014A118[p].repeated & 0x800) {
            entity_flags_apply(39, 0, 1, 0);
            D_803B28B8[p]++;
            if (D_803B28B8[p] > D_803BA050[p] - 1) D_803B28B8[p] = 0;
        } else if (D_8014A118[p].repeated & 0x3080) {
            s32 d = (D_8014A118[p].repeated & 0x1000) ? -1 : ((D_8014A118[p].repeated & 0x2000) ? 1 : 0);
            if (!D_803BA048[p] || (!d && !D_803BA04C[p])) {
                entity_flags_apply(42, 0, 1, 0);
                goto after_options;
            }
            switch (D_803BA060[p][D_803B28B8[p]]) {
            case 0:
                if (d) {
                    do {
                        D_803B9FD4[p] += d;
                        if (D_803B9FD4[p] < 0) D_803B9FD4[p] = D_803BA020[p] - 1;
                        else if (D_803B9FD4[p] >= D_803BA020[p]) D_803B9FD4[p] = 0;
                    } while (!func_800F7620(p, D_803B9FD8[p][D_803B9FD4[p]]));
                }
                D_803B9FD0[p] = D_803B9FD8[p][D_803B9FD4[p]];
                if (D_8014A110 == 6) D_8014A118[p].paint = 6;
                else D_8014A118[p].paint = func_800CDDE8((void **)D_8014A118[p].owner);
                for (i = 0; i < 13; i++) D_80110E78[p + 1][i] = D_8014A118[p].paint;
                func_8039E3BC(p);
                D_803B9BA0[p] = 1;
                break;
            case 1:
                if (D_8014A110 != 6) {
                    if (d) {
                        do {
                            D_80110E78[p + 1][D_803B9FD0[p]] += d;
                            if (D_80110E78[p + 1][D_803B9FD0[p]] < 0) D_80110E78[p + 1][D_803B9FD0[p]] = 7;
                            else if (D_80110E78[p + 1][D_803B9FD0[p]] >= 8) D_80110E78[p + 1][D_803B9FD0[p]] = 0;
                        } while (!func_800F75EC(p, D_80110E78[p + 1][D_803B9FD0[p]]) || (D_8014A110 == 6 && (D_80110E78[p + 1][D_803B9FD0[p]] & 1)));
                    } else D_80110E78[p + 1][D_803B9FD0[p]] = D_80110E78[0][D_803B9FD0[p]];
                    D_8014A118[p].paint = D_80110E78[p + 1][D_803B9FD0[p]];
                    func_800CDCAC(D_8014A118[p].owner, D_8014A118[p].paint);
                } else sound = 0;
                break;
            case 2:
                if (d) {
                    D_8011103C[p + 1][D_803B9FD0[p]] += d;
                    if (D_8011103C[p + 1][D_803B9FD0[p]] < 0) D_8011103C[p + 1][D_803B9FD0[p]] = 2;
                    else if (D_8011103C[p + 1][D_803B9FD0[p]] > 2) D_8011103C[p + 1][D_803B9FD0[p]] = 0;
                } else D_8011103C[p + 1][D_803B9FD0[p]] = D_8011103C[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 0, D_8011103C[p + 1][D_803B9FD0[p]]);
                break;
            case 3:
                if (d) {
                    do {
                        D_80111080[p + 1][D_803B9FD0[p]] += d;
                        if (D_80111080[p + 1][D_803B9FD0[p]] < 0) D_80111080[p + 1][D_803B9FD0[p]] = 8;
                        else if (D_80111080[p + 1][D_803B9FD0[p]] > 8) D_80111080[p + 1][D_803B9FD0[p]] = 0;
                    } while (!func_800F7604(p, D_80111080[p + 1][D_803B9FD0[p]]));
                } else D_80111080[p + 1][D_803B9FD0[p]] = D_80111080[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 1, D_80111080[p + 1][D_803B9FD0[p]]);
                break;
            case 4:
                if (d) {
                    do {
                        D_8011119C[p + 1][D_803B9FD0[p]] += d;
                        if (D_8011119C[p + 1][D_803B9FD0[p]] < 0) D_8011119C[p + 1][D_803B9FD0[p]] = 4;
                        else if (D_8011119C[p + 1][D_803B9FD0[p]] > 4) D_8011119C[p + 1][D_803B9FD0[p]] = 0;
                    } while (!func_800F75D0(p, D_8011119C[p + 1][D_803B9FD0[p]]));
                } else D_8011119C[p + 1][D_803B9FD0[p]] = D_8011119C[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 2, D_8011119C[p + 1][D_803B9FD0[p]]);
                break;
            case 5:
                if (d) {
                    do {
                        D_80111230[p + 1][D_803B9FD0[p]] += d;
                        if (D_80111230[p + 1][D_803B9FD0[p]] < 0) D_80111230[p + 1][D_803B9FD0[p]] = 5;
                        else if (D_80111230[p + 1][D_803B9FD0[p]] > 5) D_80111230[p + 1][D_803B9FD0[p]] = 0;
                    } while (!func_800F75B0(p, D_80111230[p + 1][D_803B9FD0[p]]));
                } else D_80111230[p + 1][D_803B9FD0[p]] = D_80111230[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 3, D_80111230[p + 1][D_803B9FD0[p]]);
                break;
            case 6:
                if (d) {
                    D_8011128C[p + 1][D_803B9FD0[p]] += d;
                    if (D_8011128C[p + 1][D_803B9FD0[p]] < 0) D_8011128C[p + 1][D_803B9FD0[p]] = (D_8014A110 != 4 ? 1 : 2);
                    else if (D_8011128C[p + 1][D_803B9FD0[p]] > (D_8014A110 != 4 ? 1 : 2)) D_8011128C[p + 1][D_803B9FD0[p]] = 0;
                } else D_8011128C[p + 1][D_803B9FD0[p]] = D_8011128C[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 4, D_8011128C[p + 1][D_803B9FD0[p]]);
                break;
            case 7:
                if (d) {
                    D_80111604[p + 1][D_803B9FD0[p]] += d;
                    if (D_80111604[p + 1][D_803B9FD0[p]] < 0) D_80111604[p + 1][D_803B9FD0[p]] = 31;
                    else if (D_80111604[p + 1][D_803B9FD0[p]] >= 32) D_80111604[p + 1][D_803B9FD0[p]] = 0;
                } else D_80111604[p + 1][D_803B9FD0[p]] = D_80111604[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 5, D_80111604[p + 1][D_803B9FD0[p]]);
                break;
            case 8:
                if (d) {
                    D_80111648[p + 1][D_803B9FD0[p]] += d;
                    if (D_80111648[p + 1][D_803B9FD0[p]] < 0) D_80111648[p + 1][D_803B9FD0[p]] = 31;
                    else if (D_80111648[p + 1][D_803B9FD0[p]] >= 32) D_80111648[p + 1][D_803B9FD0[p]] = 0;
                } else D_80111648[p + 1][D_803B9FD0[p]] = D_80111648[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 6, D_80111648[p + 1][D_803B9FD0[p]]);
                break;
            case 9:
                if (d) {
                    D_8011168C[p + 1][D_803B9FD0[p]] += d;
                    if (D_8011168C[p + 1][D_803B9FD0[p]] < 0) D_8011168C[p + 1][D_803B9FD0[p]] = 31;
                    else if (D_8011168C[p + 1][D_803B9FD0[p]] >= 32) D_8011168C[p + 1][D_803B9FD0[p]] = 0;
                } else D_8011168C[p + 1][D_803B9FD0[p]] = D_8011168C[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 7, D_8011168C[p + 1][D_803B9FD0[p]]);
                break;
            case 10:
                if (d) {
                    s32 teamFull = 1;
                    do {
                        D_8012E67C[p] += d;
                        if (D_8012E67C[p] < 0) D_8012E67C[p] = 3;
                        else if (D_8012E67C[p] > 3) D_8012E67C[p] = 0;
                        for (i = 0; i < D_8014A108; i++) {
                            if (i != p && D_8012E67C[i] != D_8012E67C[p]) teamFull = 0;
                        }
                    } while (teamFull == 1);
                } else D_8012E67C[p] = p;
                break;
            case 11:
                if (d) {
                    D_8011157C[p + 1][D_803B9FD0[p]] += d;
                    if (D_8011157C[p + 1][D_803B9FD0[p]] < 0) D_8011157C[p + 1][D_803B9FD0[p]] = 20;
                    else if (D_8011157C[p + 1][D_803B9FD0[p]] > 20) D_8011157C[p + 1][D_803B9FD0[p]] = 0;
                } else D_8011157C[p + 1][D_803B9FD0[p]] = D_8011157C[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 8, D_8011157C[p + 1][D_803B9FD0[p]]);
                break;
            case 12:
                if (d) {
                    D_801115C0[p + 1][D_803B9FD0[p]] += d;
                    if (D_801115C0[p + 1][D_803B9FD0[p]] < 0) D_801115C0[p + 1][D_803B9FD0[p]] = 9;
                    else if (D_801115C0[p + 1][D_803B9FD0[p]] > 9) D_801115C0[p + 1][D_803B9FD0[p]] = 0;
                } else D_801115C0[p + 1][D_803B9FD0[p]] = D_801115C0[0][D_803B9FD0[p]];
                sound = 0;
                scheduler_recv(D_803BA1D0[p]);
                D_803BA1D0[p] = entity_flags_apply(D_801115C0[p + 1][D_803B9FD0[p]] + 26, 0, 1, 0);
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 9, D_801115C0[p + 1][D_803B9FD0[p]]);
                break;
            case 13:
                if (d) {
                    D_801112DC[p + 1][D_803B9FD0[p]] += d * 0.05f;
                    if (D_801112DC[p + 1][D_803B9FD0[p]] < 0.74f) D_801112DC[p + 1][D_803B9FD0[p]] = 1.5f;
                    else if (D_801112DC[p + 1][D_803B9FD0[p]] > 1.6f) D_801112DC[p + 1][D_803B9FD0[p]] = 0.75f;
                } else D_801112DC[p + 1][D_803B9FD0[p]] = D_801112DC[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 10, (s8)(100.0f * D_801112DC[p + 1][D_803B9FD0[p]] - 75.0f));
                break;
            case 14:
                if (d) {
                    D_801113E0[p + 1][D_803B9FD0[p]] += d * 0.05f;
                    if (D_801113E0[p + 1][D_803B9FD0[p]] < 0.74f) D_801113E0[p + 1][D_803B9FD0[p]] = 1.5f;
                    else if (D_801113E0[p + 1][D_803B9FD0[p]] > 1.5f) D_801113E0[p + 1][D_803B9FD0[p]] = 0.75f;
                } else D_801113E0[p + 1][D_803B9FD0[p]] = D_801113E0[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 11, (s8)(100.0f * D_801113E0[p + 1][D_803B9FD0[p]] - 75.0f));
                break;
            case 15:
                if (d) {
                    D_803B28B0[p] += d;
                    if (D_803B28B0[p] < 0) D_803B28B0[p] = 10;
                    else if (D_803B28B0[p] > 10) D_803B28B0[p] = 0;
                } else D_803B28B0[p] = 0;
                sfx_stop(D_803B9FD0[p], p, D_803B28B0[p]);
                break;
            case 16: D_80152698[1] = func_8039E200(D_80152698[1], d); break;
            case 17: D_80152698[2] = func_8039E200(D_80152698[2], d); break;
            case 18: D_80152698[3] = func_8039E200(D_80152698[3], d); break;
            }
            if (sound) audio_doppler(D_8014A118[p].repeated);
        }
        if (D_803BA038[p] > D_803B28B8[p] - 1) D_803BA038[p] = D_803B28B8[p] - 1;
        if (D_803BA038[p] < D_803B28B8[p] - 1) D_803BA038[p] = D_803B28B8[p] - 1;
        if (D_803BA038[p] > D_803BA050[p] - 3) D_803BA038[p] = D_803BA050[p] - 3;
        if (D_803BA038[p] < 1) D_803BA038[p] = 1;
        D_803BA048[p] = 1;
after_options:
        if (D_803BA018[p] != D_803B9FD0[p]) {
            if (D_803BA018[p] >= 0) func_800CCA04(D_8014A118[p].owner, D_803B9FD0[p]);
            D_803BA018[p] = D_803B9FD0[p];
        }
        if (D_8014A110 == 2) {
            for (ghostIdx = 1; ghostIdx < D_803BA00C + 1; ghostIdx++) {
                if (D_803B9FC0[ghostIdx]) {
                    pos[0] = D_803AF9A8[0][38].position[0];
                    pos[1] = D_803AF9A8[0][38].position[1];
                    pos[2] = D_803AF9A8[0][38].position[2];
                    pos[1] = -pos[1] / 2.0f;
                    brake_light_update(ghostIdx, pos, &D_80150B70[ghostIdx], 0, gscreenPos);
                    gx = gscreenPos[0];
                    gy = gscreenPos[1];
                    object_create(12);
                    if (!D_80152698[ghostIdx]) {
                        if (!D_803AF980) gw = object_manager_update(countdown_state.text[225], -1);
                        else gw = object_manager_update(countdown_state.text[224], -1);
                        gh = object_bytes_sum_global();
                    } else {
                        gw = object_manager_update((*D_80152698[ghostIdx])->name, -1);
                        gw = (gw > object_manager_update(D_803B8590, -1) ? gw : object_manager_update(D_803B85A0, -1));
                        gh = object_bytes_sum_global() * 2;
                    }
                    gy -= object_bytes_sum_global() / 2;
                    crowd_cheer_play(D_803B9FC0[ghostIdx], gx - gw / 2 - 4, gy - 4, gx + gw / 2 + 4, gy + gh + 4);
                }
            }
        }
        func_8039D6A4(p);
        func_8039D300(p);
    }
    D_803BA02C = 1;
    for (i = 0; i < D_8014A108; i++) if (!D_803BA028[i]) D_803BA02C = 0;
    if (D_803BA02C && D_803BA030 >= 10) {
        speed_mode0_wrapper(0.0f, 3.0f);
        func_800B5570(0x40000);
        func_8039D05C();
        if (D_8014A110 == 3) net_session_update();
        if (D_8014A110 == 2) {
            {
                Record **gh;
                for (gh = D_80152020.first; gh; gh = (*gh)->next) menu_transition((void **)gh);
            }
            for (i = 1; i < D_803BA00C; i++) {
                if (!D_80152698[i]) { D_80152698[i] = D_80152698[i + 1]; D_80152698[i + 1] = 0; }
            }
            for (i = 1; i < D_803BA00C; i++) {
                if (!D_80152698[i]) { D_80152698[i] = D_80152698[i + 1]; D_80152698[i + 1] = 0; }
            }
        }
        pause_resume(0);
    }
}
