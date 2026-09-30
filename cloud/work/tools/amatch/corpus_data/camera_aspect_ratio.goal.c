void camera_aspect_ratio(Camera *cam) {
    CamTbl *t;

    results_screen_update(cam->handle);
    t = &D_80117530[cam->tbl];
    if (t->trackA != -1) {
        cam->handle = camera_track_entry(cam, t->f2C, t->trackA, t->s28);
    } else {
        cam->handle = -1;
    }
    cam->state = 0;
}