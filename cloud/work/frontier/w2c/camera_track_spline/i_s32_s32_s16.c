void camera_track_spline(Camera *cam) {
    f32 v[3];
    f32 w[3];
    CamCtl *ctl;
    CamScene *sc;
    CamKey *k;
    CamKey *keys;
    s32 idx;
    s32 n;
    s16 next;
    s32 near;
    f32 t;
    f32 d;
    f32 a;
    f32 b;
    f32 c;
    f32 *p;
    s32 i;

    ctl = cam->ctl;
    idx = ctl->idx;
    sc = ctl->scene;
    k = &sc->keys[idx];
    n = sc->count;
    keys = sc->keys;
    if (idx < n - 1 || (sc->flags & 2) != 0 || !(sc->flags & 0x80)) {
        next = idx + 1;
        if (k->flags & 0x10000008) {
            if (ctl->mode & 8) {
                t = k->dur - ctl->t;
            } else {
                t = ctl->t;
            }
            d = k->f34 * (t / k->dur);
        } else {
            if (next >= n && (sc->flags & 2)) {
                next = 0;
            }
            a = k->f3C;
            b = keys[next].f3C;
            if (ctl->mode & 8) {
                t = k->dur - ctl->t;
            } else {
                t = ctl->t;
            }
            c = (t * t * (b - a)) / (k->dur * 2.0f);
            if (b < a) {
                d = a * t + c;
            } else {
                d = a * t + c;
            }
        }
        v[0] = k->dir[0];
        v[1] = k->dir[1];
        v[2] = k->dir[2];
        ctl->look[0] = cam->pos[0];
        ctl->look[1] = cam->pos[1];
        ctl->look[2] = cam->pos[2];
        v[0] = v[0] * d;
        v[1] = v[1] * d;
        v[2] = v[2] * d;
        for (p = v; p < v + 3; p++) {
            i = *p * 32.0f;
            *p = i * 0.03125f;
        }
        cam->pos[0] = v[0] + k->pos[0];
        cam->pos[1] = v[1] + k->pos[1];
        cam->pos[2] = v[2] + k->pos[2];
        v[0] = cam->pos[0] - ctl->look[0];
        v[1] = cam->pos[1] - ctl->look[1];
        v[2] = cam->pos[2] - ctl->look[2];
        ctl->f10 = sqrtf(v[2] * v[2] + (v[0] * v[0] + v[1] * v[1])) / D_8002EB94;
        if (sc->flags & 0x20) {
            near = 1;
            if (gameplay_mode == 5) {
                w[0] = cam->pos[0] - player_array[0].pos[0];
                w[1] = cam->pos[1] - player_array[0].pos[1];
                w[2] = cam->pos[2] - player_array[0].pos[2];
                if (202500.0f < w[2] * w[2] + (w[0] * w[0] + w[1] * w[1])) {
                    near = 0;
                }
            }
            if (near != 0) {
                func_800C0294(sc->id, v);
            }
        }
    }
}
