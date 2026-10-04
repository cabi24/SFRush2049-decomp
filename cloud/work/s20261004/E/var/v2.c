s32 slot_state_setup(s32 selection) {
    s32 *p;
    s8 old;
    old = D_80149DA0;
    D_80149DA0 = selection;
    if (selection != -1) {
        p = &D_80149780;
        *p = func_80097694(D_80149DA0 + 0x26, -1);
        if (*p < 0) {
            *p = audio_frame_sync(D_80149DA0 + 0x26, 0, 0, 1, 0);
            display_list_alloc(*p);
        }
        p = &D_801497A4;
        *p = func_80097694(D_80149DA0 + 0x16, -1);
        if (*p < 0) {
            *p = audio_frame_sync(D_80149DA0 + 0x16, 0, 0, 0, D_80114740);
            display_list_alloc(*p);
        }
        sound_update_channel(old != selection);
    }
    if (selection == 0) {
        object_byte9_set(1);
    }
    return old;
}
