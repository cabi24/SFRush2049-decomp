void camera_build_view_matrix(s32 idx, Camera *cam) {
    CamCtl *ctl;
    CamScene *sc;
    s32 i;

    ctl = cam->ctl;
    sc = ctl->scene;
    if ((sc->flags & 1) && idx >= sc->count - 1) {
        ctl->mode = 8;
    } else {
        ctl->mode = 4;
    }
    ctl->idx = idx;
    ctl->t = 0.0f;
    ctl->f10 = 0.0f;
    cam->pos[0] = sc->keys[ctl->idx].pos[0];
    cam->pos[1] = sc->keys[ctl->idx].pos[1];
    cam->pos[2] = sc->keys[ctl->idx].pos[2];
    ctl->look[0] = sc->keys[ctl->idx].pos[0];
    ctl->look[1] = sc->keys[ctl->idx].pos[1];
    ctl->look[2] = sc->keys[ctl->idx].pos[2];
    if (sc->flags & 0xC0) {
        sc->flags |= 0x100;
        sc->cur = ctl;
    }
    if (sc->flags & 0x5000) {
        sc->flags |= 0x400;
    }
    func_800BFBE8(cam->m, sc->keys[ctl->idx].rot, 1);
    math_utility(cam->m, ctl->mat);
    func_800C0828(ctl->mat);
    for (i = 0; i < 3; i++) {
        cam->m[0][i] = sc->keys[ctl->idx].scale[0] * cam->m[0][i];
        cam->m[1][i] = sc->keys[ctl->idx].scale[1] * cam->m[1][i];
        cam->m[2][i] = sc->keys[ctl->idx].scale[2] * cam->m[2][i];
    }
}