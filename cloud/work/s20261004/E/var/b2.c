static s32 font_set(s32 font)
{
    s32 old;
    osRecvMesg(&D_801461D0, (void*)0, 1);
    old = slot_state_setup(font);
    osJamMesg(&D_801461D0, (void*)0, 0);
    return old;
}
s32 func_80100B8C(s32 arg0)
{
    render_helper(0.0f);
    D_80118E20 = 1;
    D_80118E24 = 1;
    if (D_801613E8 == 1) {
        font_set(13);
        dispatch_handler(22);
        camera_auto_follow(160, 10, 320, 110, -1, 0, D_8012029C);
        camera_auto_follow(160, 190, 320, 110, -1, 0, countdown_object[33]);
    } else {
        font_set(11);
        dispatch_handler(22);
        camera_auto_follow(160, 185, 320, 110, -1, 0, countdown_object[32]);
    }
    dispatch_handler(1);
    D_80118E24 = 3;
    D_80118E20 = 0;
    render_helper(-1.0f);
    return 1;
}
