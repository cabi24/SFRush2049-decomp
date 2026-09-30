void player_mode_set(s32 arg0, s8 arg1) {
    if (arg0 == -1) {
        D_80149B75 = arg1;
        D_80149B76 = arg1;
        D_80149B77 = arg1;
        return;
    }
    if (arg0 < 4) {
        (&D_80149B74)[arg0] = arg1;
    }
}