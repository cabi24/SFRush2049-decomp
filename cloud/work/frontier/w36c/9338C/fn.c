/* func_8039338C = controlSelect@recordscr.c (MAT3 GameSource recordscr.c, GC 0x80080874, size 0x788).
 * Direct typed port. Globals: D_803BA808 gSelectMenuSelect, D_803BA7E0 gSelectMenuValid[11], D_803BA80C
 * gSelectMenuClear, D_803BA800 gStatPlayer (tPlayerProfile **), D_803B65A4 gSelectPakIdx, D_8012E6D8 gPlayerFiles,
 * D_803BA84D gDeleteConfirm, D_803BA850 gYes, D_803BA854 gDeleteNotify, D_803BA840 gDeleteName[13], D_803BA7D4
 * buttonsJustInited, D_80146150 gJustPlayers[4], D_80149784 gActiveAutoEdges, D_80156944 gActiveLevels,
 * D_8015694C gActiveEdges. Calls: func_800B5F88 PlayLRSound, func_800B5F4C PlayUDSound, func_800B5FC4
 * PlayStartABSound, func_80393004 updateSelectInfo, func_800CD544 ClearPlayerRecords, func_800BE6A4 TB_Strcpy,
 * func_800CCEFC ReadPlayerData, func_800A3424 PS_LockFile, func_803931C0 setRecordState, func_800C9404
 * ClearTotalStats, func_800F769C AddPlayerStats, func_800C734C AddPlayerScores, func_800C7308 PurgePlayerData,
 * func_803932E4 uninitRecordScreen, func_800B5570 setGameState.
 * N64 differences: no MtapConnected test (pak index always wraps at 4), gText.single[177] (GC 187). */
void func_8039338C(void)
{
    s32 p, i;
    while (!D_803BA7E0[D_803BA808]) {
        D_803BA80C = 0;
        D_803BA808--;
        if (D_803BA808 < 0) D_803BA808 = 10;
        if (D_803BA808 >= 3 && D_803BA808 < 11 && D_803BA7E0[D_803BA808] == 1) {
            if (D_803BA800) {
                do { D_803BA800 = D_803BA800->payload->prev; }
                while (D_803BA800 && D_803BA800->payload->object->owner->slot != D_803B65A4);
            }
            if (!D_803BA800) D_803BA800 = D_8012E6D8.tail;
        } else D_803BA800 = 0;
    }
    if (D_803BA854 == 1) {
    } else if (D_803BA84D == 1) {
        if (D_80149784 & 0x3000) {
            func_800B5F88(D_80149784);
            D_803BA850 = !D_803BA850;
        }
    } else if (D_803BA808 == 1 && (D_80156944 & 0x3000)) {
        if (D_80149784 & 0x1000) {
            func_800B5F88(D_80149784);
            D_803B65A4--;
            if (D_803B65A4 < 0) D_803B65A4 = 3;
        } else if (D_80149784 & 0x2000) {
            func_800B5F88(D_80149784);
            D_803B65A4++;
            if (D_803B65A4 > 3) D_803B65A4 = 0;
        }
        func_80393004();
        D_803BA7D4 = 1;
    } else if (D_803BA808 >= 3 && D_803BA808 <= 11 && (D_80156944 & 0x3000)) {
        if (D_8015694C & 0x3000) {
            func_800B5F88(D_80149784);
            D_803BA80C = !D_803BA80C;
        }
    } else if (D_80149784 & 0x400) {
        func_800B5F4C(D_80149784);
        D_803BA80C = 0;
        do {
            if (D_803BA808 == 3 && D_803BA7E0[D_803BA808] == 1) {
                if (D_803BA800) {
                    do { D_803BA800 = D_803BA800->payload->prev; }
                    while (D_803BA800 && D_803BA800->payload->object->owner->slot != D_803B65A4);
                }
                if (D_803BA800) break;
            }
            D_803BA808--;
            if (D_803BA808 < 0) D_803BA808 = 10;
            if (D_803BA808 >= 3 && D_803BA808 < 11 && D_803BA7E0[D_803BA808] == 1) {
                if (D_803BA800) {
                    do { D_803BA800 = D_803BA800->payload->prev; }
                    while (D_803BA800 && D_803BA800->payload->object->owner->slot != D_803B65A4);
                }
                if (!D_803BA800) {
                    D_803BA800 = D_8012E6D8.tail;
                    while (D_803BA800 && D_803BA800->payload->object->owner->slot != D_803B65A4) D_803BA800 = D_803BA800->payload->prev;
                }
            } else D_803BA800 = 0;
        } while (!D_803BA7E0[D_803BA808]);
    } else if (D_80149784 & 0x800) {
        func_800B5F4C(D_80149784);
        D_803BA80C = 0;
        do {
            if (D_803BA808 == 10 && D_803BA7E0[D_803BA808] == 1) {
                if (D_803BA800) {
                    do { D_803BA800 = D_803BA800->payload->next; }
                    while (D_803BA800 && D_803BA800->payload->object->owner->slot != D_803B65A4);
                }
                if (D_803BA800) break;
            }
            D_803BA808++;
            if (D_803BA808 > 10) D_803BA808 = 0;
            if (D_803BA808 >= 3 && D_803BA808 < 11 && D_803BA7E0[D_803BA808] == 1) {
                if (D_803BA800) {
                    do { D_803BA800 = D_803BA800->payload->next; }
                    while (D_803BA800 && D_803BA800->payload->object->owner->slot != D_803B65A4);
                }
                if (!D_803BA800) {
                    D_803BA800 = D_8012E6D8.head;
                    while (D_803BA800 && D_803BA800->payload->object->owner->slot != D_803B65A4) D_803BA800 = D_803BA800->payload->next;
                }
            } else D_803BA800 = 0;
        } while (!D_803BA7E0[D_803BA808]);
    }
    if (D_8015694C & 2) {
        if (D_803BA80C == 1) {
            func_800B5FC4(D_8015694C);
            if (D_803BA854 == 1) D_803BA854 = 0;
            else if (D_803BA84D == 1) {
                if (D_803BA850 == 1) {
                    if (D_803BA800) {
                        if (func_800CD544(D_803BA800)) D_803BA854 = 1;
                    } else {
                        for (p = 0; p < 4; p++) func_800CD544(&D_80146150[p]);
                        D_803BA854 = 1;
                    }
                }
                D_803BA84D = 0;
            } else {
                if (D_803BA800) func_800BE6A4(D_803BA840, D_803BA800->payload->text);
                else func_800BE6A4(D_803BA840, D_8017A4E4->label);
                D_803BA84D = 1;
                D_803BA850 = 0;
            }
        } else if (D_803BA808 == 0 || (D_803BA808 >= 3 && D_803BA808 < 11)) {
            func_800B5FC4(D_8015694C);
            if (D_803BA800) {
                if (func_800CCEFC(D_803BA800)) {
                    func_800A3424(D_803BA800->payload->object, 1);
                    func_803931C0(1);
                }
            } else {
                ListNode *node;
                func_800C9404();
                for (i = 0; i < 4; i++) {
                    func_800F769C(&D_80146150[i]);
                    func_800C734C(&D_80146150[i]);
                }
                for (node = D_8012E6D8.head; node; node = node->payload->next) {
                    if (node->payload->flags2C) {
                        func_800F769C(node); func_800C734C(node);
                    } else {
                        if (!func_800CCEFC(node)) break;
                        func_800F769C(node); func_800C734C(node);
                        func_800C7308(node);
                    }
                }
                if (!node) func_803931C0(1);
            }
        }
    } else if (D_8015694C & 5) {
        func_800B5FC4(D_8015694C);
        if (D_803BA854 == 1) D_803BA854 = 0;
        else if (D_803BA84D == 1) D_803BA84D = 0;
        else { func_803932E4(); func_800B5570(16); }
    }
}
