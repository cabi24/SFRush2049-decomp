s32 slot_state_setup(s32 selection) {
    s8 old;
    old = D_80149DA0;
    D_80149DA0 = selection;
    if (D_80149DA0 != -1) {
        if ((D_80149780 = func_80097694(D_80149DA0 + 0x26, -1)) < 0) {
            display_list_alloc(D_80149780 = audio_frame_sync(D_80149DA0 + 0x26, 0, 0, 1, 0));
        }
        if ((D_801497A4 = func_80097694(D_80149DA0 + 0x16, -1)) < 0) {
            display_list_alloc(D_801497A4 = audio_frame_sync(D_80149DA0 + 0x16, 0, 0, 0, D_80114740));
        }
        sound_update_channel(old != selection);
    }
    if (selection == 0) {
        object_byte9_set(1);
    }
    return old;
}
