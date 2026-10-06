void camera_update(CamNode *node, s16 flag) {
    Camera *cam;
    CamCtl *ctl;
    CamScene *sc;
    CamScene *lk;
    CamCtl *lc;
    CamTbl *tb;
    s16 na;
    u16 m;
    s32 idx;
    s32 i;
    s32 nx;
    s8 moved;
    f32 dv[3];
    f32 sv[3];
    f32 f;
    s32 v;
    f32 tt;

    moved = 0;
    if (flag == 0) {
        entity_transform_apply(node, 1);
        return;
    }
    if (((state_word_a & 0x7C0000) || (state_word_a & 8)) && D_801170FC == 0) {
        cam = node->cam;
        ctl = cam->ctl;
        sc = ctl->scene;
        if (sc->flags & 0x40) {
            if (sc->flags & 0x4000) {
                if (!(sc->flags & 0x200)) {
                    if (!(sc->flags & 0x400)) {
                        if (ctl->mode & 4) {
                            ctl->mode &= ~4;
                            ctl->mode |= 8;
                            ctl->t = sc->keys[ctl->idx].dur - ctl->t;
                        }
                        if (sc->flags & 0x100) {
                            camera_aspect_ratio(cam);
                        }
                        sc->flags = sc->flags & ~0x100;
                    }
                } else {
                    if (sc->flags & 0x400) {
                        sc->flags = sc->flags & ~0x500;
                        camera_aspect_ratio(cam);
                    }
                    sc->flags = sc->flags & ~0x200;
                }
            } else {
                if ((sc->flags & 0x2000) && (sc->flags & 0x100000)) {
                    if (sc->link->flags & 0x100) {
                        sc->flags &= ~0x100000;
                        sc->flags |= 0x200400;
                        if (!(sc->flags & 0x8000)) {
                            func_800C15FC(cam->slot, D_80142A7A);
                        }
                        camera_fov_control(cam);
                        return;
                    }
                }
                if ((sc->flags & 0x400) && (sc->flags & 0x200)) {
                    lk = sc->link;
                    if (lk->flags & 0x100) {
                        lk->flags &= ~0x100;
                    } else if (sc->flags & 0x1000) {
                        lc = lk->cur;
                        if (lc->mode & 8) {
                            lc->mode &= ~8;
                            lc->mode |= 4;
                            lc->t = lk->keys[lc->idx].dur - lc->t;
                        } else {
                            lc->mode &= ~4;
                            lc->mode |= 8;
                            lc->t = lk->keys[lc->idx].dur - lc->t;
                        }
                    }
                    sc->flags = sc->flags & ~0x700;
                    if (sc->flags & 0x1000) {
                        if (sc->flags & 0x200000) {
                            sc->flags &= ~0x200000;
                            sc->flags |= 0x100000;
                            if (!(sc->flags & 0x8000)) {
                                func_800C15FC(cam->slot, D_80142A78);
                            }
                            camera_aspect_ratio(cam);
                        } else {
                            sc->flags &= ~0x100000;
                            sc->flags |= 0x200000;
                            if (!(sc->flags & 0x8000)) {
                                func_800C15FC(cam->slot, D_80142A7A);
                            }
                            camera_fov_control(cam);
                        }
                    } else if (sc->flags & 0x200000) {
                        sc->flags &= ~0x200000;
                        sc->flags |= 0x100000;
                        if (!(sc->flags & 0x8000)) {
                            func_800C15FC(cam->slot, D_80142A78);
                        }
                        camera_aspect_ratio(cam);
                    }
                }
            }
        }
        if (!(sc->flags & 0x100)) {
            i = 0;
            ctl->t += *(f32 *)(u32)&D_8002EB94;
            idx = ctl->idx;
            do {
                f = sc->keys[idx].dur;
                if (ctl->t >= f) {
                    ctl->t = ctl->t - f;
                    if (ctl->mode & 8) {
                        if (idx == 0) {
                            if (sc->flags & 0x4000) {
                                camera_build_view_matrix(0, cam);
                                i = 1;
                                camera_fov_control(cam);
                            } else {
                                ctl->mode &= ~8;
                                ctl->mode |= 4;
                                if (sc->flags & 0x80) {
                                    sc->flags |= 0x100;
                                    i = 1;
                                    ctl->t = 0.0f;
                                }
                            }
                            if (sc->flags & 0x20) {
                                listener_position_set(sc->id);
                            }
                        } else {
                            ctl->idx = idx - 1;
                        }
                    } else if (idx + 1 == sc->count) {
                        if (sc->flags & 0x80) {
                            ctl->mode &= ~4;
                            ctl->mode |= 8;
                            ctl->idx = idx - 1;
                        } else {
                            ctl->idx = 0;
                            if (sc->flags & 0x20) {
                                listener_position_set(sc->id);
                            }
                        }
                    } else {
                        ctl->idx = idx + 1;
                        idx = ctl->idx;
                        if (sc->keys[idx].flags & 0x40) {
                            i = 1;
                            sc->flags |= 0x100;
                            ctl->t = 0.0f;
                        } else if (sc->count == idx + 1) {
                            if (sc->flags & 1) {
                                ctl->mode &= ~4;
                                ctl->mode |= 8;
                                ctl->idx = idx - 1;
                            } else if (sc->flags & 0x4000) {
                                ctl->mode &= ~4;
                                ctl->mode |= 8;
                                ctl->idx = idx - 1;
                                ctl->t = 0.0f;
                                sc->flags |= 0x100;
                            } else if (!(sc->flags & 2)) {
                                camera_build_view_matrix(0, cam);
                                if (sc->flags & 0x20) {
                                    listener_position_set(sc->id);
                                }
                            }
                        }
                    }
                    idx = ctl->idx;
                } else {
                    i = 1;
                }
            } while (i == 0);
            v = sc->keys[idx].flags;
            if (!(v & 2)) {
                camera_track_spline(cam);
                moved = 1;
                v = sc->keys[ctl->idx].flags;
            }
            if (!(v & 0x10) || !(v & 4)) {
                moved = 1;
                camera_free_look(cam);
            }
            if (moved != 0) {
                if (sc->flags & 0x100) {
                    camera_fov_control(cam);
                } else {
                    camera_look_at_point(cam);
                }
            } else {
                camera_fov_control(cam);
            }
            tb = &D_80117530[cam->tbl];
            if (tb->s14 != -1) {
                node->f10 -= *(f32 *)(s32)&D_8002EB94;
                if (node->f10 <= 0.0f) {
                    v = tb->s14;
                    if (v == 0x169) {
                        node->f10 = 0.25f;
                    } else if (v == 0x16D) {
                        node->f10 = D_80123E8C;
                    }
                    v = ++node->s04;
                    if (v >= cam->s5A) {
                        node->s04 = 0;
                        v = 0;
                    }
                    na = cam->s58 + v;
                    if (na != cam->s50) {
                        m = (&D_801427C0)[na];
                        if (sc->flags & 0x8000) {
                        } else {
                            func_800C15FC(cam->slot, m);
                        }
                        cam->s50 = na;
                    }
                    goto block_99;
                }
            } else {
block_99:
                if (tb->mode == 4) {
                    if (ctl->mode & 8) {
                        dv[0] = sc->keys[ctl->idx].dir[0] * -ctl->f10;
                        dv[1] = sc->keys[ctl->idx].dir[1] * -ctl->f10;
                        dv[2] = sc->keys[ctl->idx].dir[2] * -ctl->f10;
                    } else {
                        dv[0] = sc->keys[ctl->idx].dir[0] * ctl->f10;
                        dv[1] = sc->keys[ctl->idx].dir[1] * ctl->f10;
                        dv[2] = sc->keys[ctl->idx].dir[2] * ctl->f10;
                    }
                    func_800AB750(cam->s65, dv, cam->pos, cam->m);
                }
                idx = ctl->idx;
                if (!(sc->keys[idx].flags & 0x10)) {
                    nx = idx + 1;
                    if (nx >= sc->count) {
                        nx = 0;
                    }
                    if (ctl->mode & 8) {
                        f = sc->keys[idx].dur;
                        tt = f - ctl->t;
                    } else {
                        tt = ctl->t;
                        f = sc->keys[idx].dur;
                    }
                    for (i = 0; i < 3; i++) {
                        sv[i] = sc->keys[ctl->idx].scale[i] + (sc->keys[nx].scale[i] - sc->keys[ctl->idx].scale[i]) * (tt / f);
                    }
                    for (i = 0; i < 3; i++) {
                        cam->m[0][i] *= sv[0];
                        cam->m[1][i] *= sv[1];
                        cam->m[2][i] *= sv[2];
                    }
                }
                if (!(cam->flags & 2)) {
                    if (!(sc->flags & 0x8000)) {
                        entity_spawn_callback(cam->slot, 0, 0);
                    }
                    func_800AFA84(&D_80143FC8, (s32 *) cam);
                    entity_transform_apply(node, 1);
                }
            }
        }
    }
}
