void camera_free_look(Camera *cam) {
    f32 out[4];
    f32 d[3];
    CamCtl *ctl;
    CamScene *sc;
    CamKey *k;
    f32 t;
    f32 dur;
    s32 next;
    s32 near;
    f32 dist;

    ctl = cam->ctl;
    sc = cam->ctl->scene;
    k = &sc->keys[ctl->idx];
    if (k->flags & 0x20) {
        goto plain;
    }
    if (ctl->mode & 8) {
        t = k->dur - ctl->t;
    } else {
        t = ctl->t;
    }
    dur = k->dur;
    if (dur == 0.0f) {
plain:
        func_800BFBE8(cam->m[0], k->rot, 1);
    } else {
        next = ctl->idx + 1;
        if (next >= sc->count && (sc->flags & 2)) {
            next = 0;
        }
        func_800BFD8C(t / dur, k->rot, sc->keys[next].rot, out);
        func_800BFBE8(cam->m[0], out, 1);
    }
    if (sc->flags & 0x20) {
        near = 1;
        if (gameplay_mode == 5) {
            d[0] = cam->pos[0] - ((f32 *) &player_array)[2];
            d[1] = cam->pos[1] - ((f32 *) &player_array)[3];
            d[2] = cam->pos[2] - ((f32 *) &player_array)[4];
            dist = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
            if (D_80123E84 < dist) {
                near = 0;
            }
        }
        if (near != 0) {
            camera_first_person(sc->id, cam->pos, ctl->mat, cam->m[0]);
        }
    }
}
