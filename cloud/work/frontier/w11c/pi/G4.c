void camera_process_input(Camera *cam) {
    s32 unused[11];
    u8 col[4] = { 255, 0, 0, 255 };
    CamTbl *tbl;
    CamScene *sc;
    CamCtl *ctl;
    s32 i;
    f32 mx;
    CamNode *node;
    s32 id;
    s32 fl;

    cam->state = 2;
    tbl = &D_80117530[cam->tbl];
    ctl = cam->ctl;
    sc = cam->ctl->scene;
    if (!(cam->flags & 1)) {
        ctl->mode = 0;
        ctl->t = 0.0f;
        if (gameplay_mode == 2 && sc->id > 0) {
            CamScene *s;
            s32 j;
            f32 v[3];
            f32 delta[3];
            f32 mat[9];

            s = cam->ctl->scene;
            for (j = 0; j < s->count; j++) {
                CamKey *k = &s->keys[j];

                if (s->keys[j].flags & 0x1000000) {
                    cam->pos[0] = k->pos[0];
                    cam->pos[1] = k->pos[1];
                    cam->pos[2] = k->pos[2];
                    func_800BFBE8(cam->m, k->rot, 1);
                }
            }
            if (s->flags & 0x20) {
                v[0] = s->keys[0].pos[0];
                v[1] = s->keys[0].pos[1];
                v[2] = s->keys[0].pos[2];
                func_800BFBE8(mat, s->keys[0].rot, 1);
                func_800C0828(mat);
                delta[0] = cam->pos[0] - v[0];
                delta[1] = cam->pos[1] - v[1];
                delta[2] = cam->pos[2] - v[2];
                func_800C0294(s->id, delta);
                camera_first_person(s->id, cam->pos, mat, cam->m);
            }
        } else {
            if (sc->flags & 0x20) {
                D_8013C300[D_8013F1DC++] = cam;
            }
            mx = 0.0f;
            for (i = 0; i < sc->count; i++) {
                if (mx < sc->keys[i].f3C) {
                    mx = sc->keys[i].f3C;
                }
            }
            ctl->f14 = mx;
            for (i = 0; i < sc->count; i++) {
                if ((sc->keys[i].flags & 1) && !(sc->keys[i].flags & 0x1000)) {
                    camera_build_view_matrix(i, cam);
                    sc->keys[i].flags |= 0x1000;
                    i = sc->count;
                }
            }
            id = tbl->s14;
            if (361 == id) {
                cam->s5A = 4;
            } else if (365 == id) {
                cam->s5A = 20;
            }
            if ((sc->flags & 0x40) && !(sc->flags & 0x4000)) {
                sc->flags &= ~0x100000;
                sc->flags |= 0x200400;
                if (!(sc->flags & 0x8000)) {
                    func_800C15FC(cam->slot, D_80142A7A);
                }
            }
            node = func_80090284();
            if (node != NULL) {
                node->s04 = 0;
                node->w14 = tbl->w0C;
                node->cam = cam;
                id = tbl->s14;
                if (361 == id) {
                    node->f10 = 0.25f;
                } else if (365 == id) {
                    node->f10 = 0.0425f;
                }
                node->next = D_801391F0;
                D_801391F0 = node;
            }
        }
        cam->flags |= 1;
    }
}
