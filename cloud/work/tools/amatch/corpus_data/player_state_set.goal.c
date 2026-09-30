void player_state_set(s32 player, s32 value)
{
    s32 i;

    if (player == -1) {
        for (i = 0; i < 4; i++) {
            D_80149B64[i] = value;
        }
    } else if (player < 4) {
        D_80149B64[player] = value;
    }
}