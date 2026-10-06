void camera_look_at_point(Camera *cam) {
    CamTbl *t;
    f32 f;
    f32 r;
    s32 a;
    s32 b;
    s32 x;
    s32 y;
    s32 tt;

    if (cam->state == 1) {
        if (cam->handle != -1) {
            if (D_80117530[cam->tbl].s20 == 1) {
                b = (s32) cam->ctl->f14;
                a = (s32) cam->ctl->f10;
                if (b == 0) {
                    b = a != 0 ? a : 1;
                }
tt = a;
if (b < 0) {
 if (tt) {}
 if (tt) {}
 }
 if (tt) {}
                x = a < 0 ? -a : a;
                y = b < 0 ? -b : b;
                r = ((f32) x / (f32) y) * 0.5f + .5f;
                f = r;
                if (r < 0.0f || r > 1.0f) {
                    if (r < 0.0f) {
                        f = 0.0f;
                    } else {
                        f = 1.0f;
                    }
                }
            } else {
                f = 1.0f;
            }
            if (D_80117530[cam->tbl].s20 == 0x12) {
                camera_clip_planes(cam->handle, (s32) cam->pos, (s32) &D_801141B0, 0.8f, 1.0f);
                return;
            }
            if (D_80117530[cam->tbl].s20 == 0x61) {
                camera_clip_planes(cam->handle, (s32) cam->pos, (s32) &D_801141B0, 1.0f, 0.75f);
                return;
            }
            camera_clip_planes(cam->handle, (s32) cam->pos, (s32) &D_801141B0, 0.75f * f + 0.25f, f);
        }
    } else {
        if (cam->state == 2) {
            camera_aspect_ratio(cam);
            return;
        }
        if (leaderboard_update(cam->handle) == 0) {
            results_screen_update(cam->handle);
            t = &D_80117530[cam->tbl];
            if (t->s20 != -1) {
                cam->handle = camera_track_entry(cam, t->f2C, t->s20, t->s28);
                if (D_80117530[cam->tbl].s20 == 1) {
                    camera_clip_planes(cam->handle, (s32) cam->pos, (s32) &D_801141B0, 0.0f, 1.0f);
                }
            } else {
                cam->handle = -1;
            }
            cam->state = 1;
        }
    }
}
