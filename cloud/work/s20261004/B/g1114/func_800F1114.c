typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

extern s16 active_player_count;
extern s32 state_word_b;
extern u8 D_801148A4;
extern s8 *D_80146208;
extern s8 *D_80149418[];
extern s8 *D_80149778[2];
extern s8 *D_80149798[3];

void sound_handles_array_clear(s8 *arg0);

void func_800F1114(void)
{
    s32 i;

    if (D_80146208 != 0) {
        sound_handles_array_clear(D_80146208);
        D_80146208 = 0;
    }
    for (i = 0; i < active_player_count; i++) {
        if (D_80149418[i] != 0) {
            sound_handles_array_clear(D_80149418[i]);
            D_80149418[i] = 0;
        }
    }
    for (i = 0; i < 2; i++) {
        if (D_80149778[i] != 0) {
            sound_handles_array_clear(D_80149778[i]);
            D_80149778[i] = 0;
        }
    }
    for (i = 0; i < 3; i++) {
        if (D_80149798[i] != 0) {
            sound_handles_array_clear(D_80149798[i]);
            D_80149798[i] = 0;
        }
    }
    D_801148A4 = 0;
    state_word_b = 0x800000;
}

void __standin_func_800F1114(void)
{
    func_800F1114();
}

void __standin2_func_800F1114(void)
{
    func_800F1114();
}
