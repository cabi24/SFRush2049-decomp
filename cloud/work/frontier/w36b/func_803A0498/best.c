/* func_803A0498 = setGhosts@carscr.c (MAT3 reference/repos/mat3/decomp/src/GameSource/src/carscr.c, GC 0x80090638,
 * size 0xac0), w36b: direct typed port of the MAT3 body (tGhostFile = Record, gPlayers[0].save = D_8014A118[0].owner,
 * gGhostFiles.head = D_80152020.first, limboGhosts = D_803BA010, gGhosts = D_80152698, gLastCar = D_803BA018,
 * gGhostSelect = D_801426EC, gTrackNum = D_8014978C, gBackwards = D_80152570, strcmp = func_8008AD04,
 * ReadGhostData = menu_item_value_get). Private-ABI member of this IPA group (clobbers s0-s8; callers
 * func_803A0FD8 _InitScreen and func_803A1EAC updateCarScreen).
 * N64 differences from the GC body (from the retail words): the three clearing loops run gGhosts, nearGhosts,
 * gLastCar (GC: nearGhosts first); the player-id compares are written player-first (operand order of the two
 * lw); the best-branch limbo loop uses one `continue` guard (the reload of limboGhosts+3 is then inserted only
 * after the body's calls, as retail); an N64-only tail purges ghost slots 2 and 3 (menu_transition =
 * PurgeGhostData) when D_80156994 is clear. The shift loops have their body on its own source line: as1 breaks
 * scheduling ties by line number, and the one-line form orders the unrolled copies 0,1,2,3 instead of retail's
 * 1,2,3,0. */
void func_803A0498(void)
{
    Record **nearGhosts[5];
    Record **ghosth;
    Record *ghost;
    Record **best = 0;
    Record **nextGhost;
    Profile *player;
    s32 ghostCount;
    s32 i;
    s32 j;
    player = *D_8014A118[0].owner;
    for (i = 0; i < 4; i++) D_80152698[i] = 0;
    for (i = 0; i < 5; i++) nearGhosts[i] = 0;
    for (i = 1; i < 4; i++) D_803BA018[i] = -1;
    if (D_8014A110 != 2) return;
    if (!D_801426EC) {
        for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
            ghost = *ghosth;
            nextGhost = ghost->next;
            if (ghost->track == D_8014978C && ghost->mode == D_80152570 &&
                !func_8008AD04(ghost->name, player->name) &&
                player->id0 == ghost->id0 && player->id1 == ghost->id1) {
                if ((!best || ghost->time < (*best)->time) && menu_item_value_get(ghosth)) best = ghosth;
            }
        }
        for (i = 0; i < 3; i++) {
            ghosth = D_803BA010 + i;
            ghost = *ghosth;
            if (ghost->active && ghost->track == D_8014978C && ghost->mode == D_80152570 &&
                !func_8008AD04(ghost->name, player->name) &&
                player->id0 == ghost->id0 && player->id1 == ghost->id1) {
                if ((!best || ghost->time < (*best)->time) && menu_item_value_get(ghosth)) best = ghosth;
            }
        }
        if (!best) {
            for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
                ghost = *ghosth;
                nextGhost = ghost->next;
                if (ghost->track == D_8014978C && ghost->mode == D_80152570) {
                    for (i = 3; i >= 1; i--) {
                        if ((!D_80152698[i] || ghost->time > (*D_80152698[i])->time) && menu_item_value_get(ghosth)) {
                            for (j = 1; j < i; j++)
                                D_80152698[j] = D_80152698[j+1];
                            D_80152698[i] = ghosth;
                            break;
                        }
                    }
                }
            }
            for (ghosth = D_803BA010; ghosth < D_803BA010 + 3; ghosth++) {
                ghost = *ghosth;
                if (ghost->active && ghost->track == D_8014978C && ghost->mode == D_80152570) {
                    for (i = 3; i >= 1; i--) {
                        if ((!D_80152698[i] || ghost->time > (*D_80152698[i])->time) && menu_item_value_get(ghosth)) {
                            for (j = 1; j < i; j++)
                                D_80152698[j] = D_80152698[j+1];
                            D_80152698[i] = ghosth;
                            break;
                        }
                    }
                }
            }
            for (i = 1; i < 3; i++) {
                if (!D_80152698[i] && D_80152698[i+1]) {
                    D_80152698[i] = D_80152698[i+1];
                    D_80152698[i+1] = 0;
                    i = 0;
                }
            }
        } else {
            nearGhosts[2] = best;
            for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
                ghost = *ghosth;
                nextGhost = ghost->next;
                if (ghost->track == D_8014978C && ghost->mode == D_80152570) {
                    if (ghosth != best) {
                        if (ghost->time < (*best)->time) {
                            if (!nearGhosts[1] || ghost->time > (*nearGhosts[1])->time) {
                                if (menu_item_value_get(ghosth)) {
                                    nearGhosts[0] = nearGhosts[1];
                                    nearGhosts[1] = ghosth;
                                }
                            } else if (!nearGhosts[0] || ghost->time > (*nearGhosts[0])->time) {
                                if (menu_item_value_get(ghosth)) nearGhosts[0] = ghosth;
                            }
                        } else {
                            if (!nearGhosts[3] || ghost->time < (*nearGhosts[3])->time) {
                                if (menu_item_value_get(ghosth)) {
                                    nearGhosts[4] = nearGhosts[3];
                                    nearGhosts[3] = ghosth;
                                }
                            } else if (!nearGhosts[4] || ghost->time < (*nearGhosts[4])->time) {
                                if (menu_item_value_get(ghosth)) nearGhosts[4] = ghosth;
                            }
                        }
                    }
                }
            }
            for (ghosth = D_803BA010; ghosth < D_803BA010 + 3; ghosth++) {
                ghost = *ghosth;
                if (!ghost->active || ghost->track != D_8014978C || ghost->mode != D_80152570 || ghosth == best) continue;
                if (ghost->time < (*best)->time) {
                    if (!nearGhosts[1] || ghost->time > (*nearGhosts[1])->time) {
                        if (menu_item_value_get(ghosth)) {
                            nearGhosts[0] = nearGhosts[1];
                            nearGhosts[1] = ghosth;
                        }
                    } else if (!nearGhosts[0] || ghost->time > (*nearGhosts[0])->time) {
                        if (menu_item_value_get(ghosth)) nearGhosts[0] = ghosth;
                    }
                } else {
                    if (!nearGhosts[3] || ghost->time < (*nearGhosts[3])->time) {
                        if (menu_item_value_get(ghosth)) {
                            nearGhosts[4] = nearGhosts[3];
                            nearGhosts[3] = ghosth;
                        }
                    } else if (!nearGhosts[4] || ghost->time < (*nearGhosts[4])->time) {
                        if (menu_item_value_get(ghosth)) nearGhosts[4] = ghosth;
                    }
                }
            }
            ghostCount = 1;
            for (i = 0; i < 5; i++) {
                if (nearGhosts[i] && ghostCount < 4) { D_80152698[ghostCount] = nearGhosts[i]; ghostCount++; }
            }
        }
    } else {
        for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
            ghost = *ghosth;
            nextGhost = ghost->next;
            if (ghost->track == D_8014978C && ghost->mode == D_80152570) {
                for (i = 1; i < 4; i++) {
                    if ((!D_80152698[i] || ghost->time < (*D_80152698[i])->time) && menu_item_value_get(ghosth)) {
                        for (j = 3; j > i; j--)
                            D_80152698[j] = D_80152698[j-1];
                        D_80152698[i] = ghosth;
                        break;
                    }
                }
            }
        }
        for (ghosth = D_803BA010; ghosth < D_803BA010 + 3; ghosth++) {
            ghost = *ghosth;
            if (ghost->active && ghost->track == D_8014978C && ghost->mode == D_80152570) {
                for (i = 1; i < 4; i++) {
                    if ((!D_80152698[i] || ghost->time < (*D_80152698[i])->time) && menu_item_value_get(ghosth)) {
                        for (j = 3; j > i; j--)
                            D_80152698[j] = D_80152698[j-1];
                        D_80152698[i] = ghosth;
                        break;
                    }
                }
            }
        }
    }
    if (!D_80156994) {
        if (D_80152698[2]) { menu_transition((void **)D_80152698[2]); D_80152698[2] = 0; }
        if (D_80152698[3]) { menu_transition((void **)D_80152698[3]); D_80152698[3] = 0; }
    }
}
