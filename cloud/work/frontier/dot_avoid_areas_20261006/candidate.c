/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* NONMATCH research: N64 adaptation of rushtherock game/maxpath.c:avoid_areas,
 * historicalsource/rushtherock commit 845329d7b36f5a384c5625ed9a0aef584ab46139.
 * repro.py supplies the current real E4B58/E451C/E4300 caller context and
 * current accepted vector/obstacle helper bodies. This file is not standalone.
 *
 * The unused tmp2 scalar and temp, dir_list, dir_weight, cur_rate arrays are donor
 * declarations, not invented frame padding. MAX_LINKS=6 has separate N64
 * evidence in src/blob/func_800EC914.c and groups/render_large_objects/group.c.
 * Their combined sizes reproduce the native direction/saved-direction gap;
 * retaining this exact donor subset in the N64 version remains a hypothesis.
 * Other unused arcade declarations are not carried over. No array-size search,
 * artificial caller, false prototype, volatile, or assembly is used.
 *
 * Correct native frame (288 bytes), but 583/599 words differ, 561 emitted words,
 * with unresolved own-rodata placement caused by the broad instruction mismatch.
 * This is not a match, a byte-credit claim, or image/ROM verification.
 */
void func_800E398C(s16 drone_index) {
    s16 i, index, can_we_cheat, hint;
    f32 dist, scale, scale1, scale2, tmp2;
    f32 rpos[3];
    f32 pos[3], tpos[3], own_rwr[3], dir[3], temp[3];
    f32 dir_list[6][3], dir_weight[6];
    f32 cur_rate[3], save_dir[3];
    Nav *cp;
    D_8014A250_Record *m;

    m = &D_8014A250[drone_index];
    cp = (Nav *) ((u8 *) &player_array[drone_index] + 0x314);
    tpos[0] = cp->tgt[0];
    tpos[1] = cp->tgt[1];
    tpos[2] = cp->tgt[2];
    pos[0] = tpos[0] - m->pos[0];
    pos[1] = tpos[1] - m->pos[1];
    pos[2] = tpos[2] - m->pos[2];
    func_800A61B0(pos, rpos, (u8 *) m + 0x7A0);
    i = 0;
    if ((m->f638 == 0.0f) || (m->speed < 0.100000001f)) {
        D_8014A250[drone_index].b7DE = 0;
        rpos[0] = 0.0f;
        cp->spd = 100.0f;
        cp->tgt[0] = rpos[0];
        cp->tgt[1] = rpos[1];
        cp->tgt[2] = rpos[2];
        do {
            D_80153F88[drone_index][i] = 0.0f;
            i++;
        } while (i < 3);
        return;
    }
    if (m->speed < ((f32) (u32) D_8012E5E8[cp->sel].points[cp->pt].flag * 1.4666667f) * 0.5f) {
        rpos[0] = rpos[0] * 0.200000003f;
        cp->spd = ((f32) (u32) D_8012E5E8[cp->sel].points[cp->pt].flag * 1.4666667f) * 1.5f;
        cp->tgt[0] = rpos[0];
        cp->tgt[1] = rpos[1];
        cp->tgt[2] = rpos[2];
        i = 0;
        do {
            D_80153F88[drone_index][i] = 0.0f;
            i++;
        } while (i < 3);
        D_8014A250[drone_index].b7DE = 0;
        return;
    }
    if (rpos[2] < fabsf(rpos[0] + rpos[0])) {
        cp->tgt[0] = rpos[0];
        cp->tgt[1] = rpos[1];
        cp->tgt[2] = rpos[2];
        i = 0;
        do {
            D_80153F88[drone_index][i] = 0.0f;
            i++;
        } while (i < 3);
        D_8014A250[drone_index].b7DE = 0;
        return;
    }
    /* Retail passes the transformed local vector, not the car object. */
    dist = func_8008B3C8(rpos);
    scale = m->speed * m->f634 / dist;
    dir[0] = rpos[0] * scale;
    save_dir[0] = dir[0];
    dir[1] = rpos[1] * scale;
    save_dir[1] = dir[1];
    dir[2] = rpos[2] * scale;
    save_dir[2] = dir[2];
    own_rwr[0] = m->pos[0];
    own_rwr[1] = m->pos[1];
    own_rwr[2] = m->pos[2];
    scale1 = 1.0f;
    can_we_cheat = 1;
    i = 0;
    if (D_80152744 > 0) {
        do {
            index = D_8014A250[i].unk7C6;
            if (index != drone_index) {
                m = &D_8014A250[index];
                pos[0] = m->pos[0] - own_rwr[0];
                pos[1] = m->pos[1] - own_rwr[1];
                pos[2] = m->pos[2] - own_rwr[2];
                func_800A61B0(pos, rpos, (u8 *) &D_8014A250[drone_index] + 0x7A0);
                if (((rpos[2] * rpos[2]) + (rpos[0] * rpos[0])) <= 90000.0f) {
                    if (m->b7CC == 2) {
                        if ((fabsf(rpos[0]) < 60.0f) && (fabsf(rpos[2]) < 200.0f)) {
                            can_we_cheat = 0;
                        }
                    }
                    if (!(rpos[2] < 0.0f)) {
                        if (!(8.0f < fabsf(rpos[0]))) {
                            if (rpos[2] < 20.0f) {
                                scale2 = 0.899999976f;
                            } else if (rpos[2] < 100.0f) {
                                scale2 = 1.0f - ((100.0f - rpos[2]) * 0.00125000032f);
                            } else {
                                scale2 = 1.0f;
                            }
                            if (scale2 < scale1) {
                                scale1 = scale2;
                            }
                        }
                    }
                }
            }
            i++;
        } while (i < D_80152744);
    }
    camera_blend_between(&D_8014A250[drone_index], &scale1);
    if ((fabsf(dir[0]) * 4.0f) < fabsf(dir[2])) {
        if (dir[0] < 0.0f) {
            dir[0] += (1.0f - scale1) * 8.0f;
        } else {
            dir[0] -= (1.0f - scale1) * 8.0f;
        }
    } else if (scale1 < 0.899999976f) {
        if (dir[0] < 0.0f) {
            dir[0] = (1.0f - scale1) * 4.0f;
        } else {
            dir[0] = -(1.0f - scale1) * 4.0f;
        }
    }
    hint = ((s8 *) &D_8012E5E8[cp->sel].points[cp->pt])[7];
    if ((D_80153F88[drone_index][0] == 0.0f) && (D_80153F88[drone_index][2] == 0.0f)) {
        dir[0] = 0.0f;
        cp->spd = cp->spd * (1.0f - ((1.0f - scale1) * 0.300000012f));
    } else if ((hint == 5) || (D_8014A250[drone_index].s6C4 >= 0)) {
        dir[0] = dir[0] * 0.400000006f;
    } else if ((hint == 2) || ((D_80153F88[drone_index][0] == 0.0f) && (D_80153F88[drone_index][2] == 0.0f))) {
        dir[0] = save_dir[0];
        dir[1] = save_dir[1];
        dir[2] = save_dir[2];
        cp->spd = cp->spd * (1.0f - ((1.0f - scale1) * 0.300000012f));
    } else if (hint == 1) {
        if ((D_80153F88[drone_index][0] == 0.0f) && (D_80153F88[drone_index][2] == 0.0f)) {
            dir[0] = save_dir[0];
            dir[1] = save_dir[1];
            dir[2] = save_dir[2];
        } else {
            dir[0] = (save_dir[0] * 0.150000006f) + (D_80153F88[drone_index][0] * 0.850000024f);
        }
        cp->spd = cp->spd * (1.0f - ((1.0f - scale1) * 0.100000001f));
    } else if (hint == 3) {
        dir[0] = save_dir[0] - D_80154138[drone_index][0];
        save_dir[0] = D_80154138[drone_index][0];
        save_dir[1] = D_80154138[drone_index][1];
        save_dir[2] = D_80154138[drone_index][2];
    } else if (hint == 4) {
        dir[0] = save_dir[0] - D_80154138[drone_index][0];
    } else {
        dir[0] = (D_80153F88[drone_index][0] * 0.5f) + (dir[0] * 0.5f);
    }
    D_8014A250[drone_index].b7DE = can_we_cheat;
    cp->tgt[0] = dir[0];
    cp->tgt[1] = dir[1];
    cp->tgt[2] = dir[2];
    D_80153F88[drone_index][1] = dir[1];
    D_80153F88[drone_index][0] = dir[0];
    D_80153F88[drone_index][2] = dir[2];
    D_80154138[drone_index][1] = save_dir[1];
    D_80154138[drone_index][2] = save_dir[2];
    D_80154138[drone_index][0] = save_dir[0];
}

