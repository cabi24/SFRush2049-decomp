void camera_fov_control(Camera *cam) {
    CamTbl *t;

    if (cam->state != 2 && (cam->state != 0 || leaderboard_update(cam->handle) == 0)) {
        results_screen_update(cam->handle);
        t = &D_80117530[cam->tbl];
        if (t->trackB != -1) {
            cam->handle = camera_track_entry(cam, t->f2C, t->trackB, t->s28);
        } else {
            cam->handle = -1;
        }
        cam->state = 2;
    }
}