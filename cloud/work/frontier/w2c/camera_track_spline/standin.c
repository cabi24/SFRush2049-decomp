
/* STAND-IN caller (not real): two call sites that keep the camera in a3 */
extern s32 D_STANDIN;
void camera_update(s32 a, s32 b, s32 c, Camera *cam)
{
    if (a) {
        camera_track_spline(cam);
    } else {
        camera_track_spline(cam);
        D_STANDIN = b + c;
    }
}
