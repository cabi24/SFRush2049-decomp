void player_state_set(s32 arg0, s8 arg1) {
    if (arg0 == -1) {
        D_80149B65 = arg1;
        D_80149B66 = arg1;
        D_80149B67 = arg1;
        return;
    }
    if (arg0 < 4) {
        (&D_80149B64)[arg0] = arg1;
    }
}