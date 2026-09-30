void player_mode_set(s32 player, s32 value)
{
    s32 i;

    if (player == -1) {
        for (i = 0; i < 4; i++) {
            D_80149B74[i] = value;
        }
    } else if (player < 4) {
        D_80149B74[player] = value;
    }
}