typedef signed short s16; typedef unsigned char u8; typedef signed int s32; typedef unsigned int u32;
extern u32 object_manager_update(void *,s16);
extern void race_position_update(void *,void *,s16),state_utility(s16,s16,void *);
s16 sound_pitch_diff_halved(void *text, s16 centerX)
{
    return centerX - (object_manager_update(text, -1) >> 1);
}

s16 sound_pitch_diff_calc(void *text, s16 rightX)
{
    return rightX - object_manager_update(text, -1);
}

void music_fade(s16 x, s16 y, s16 w, void *text)
{
    u8 label[128];
    race_position_update(label, text, w);
    state_utility(x, y, label);
}
