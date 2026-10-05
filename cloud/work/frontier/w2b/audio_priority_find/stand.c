
extern s32 D_standin[];
void standin_caller(s16 a, s16 b) {
    D_standin[0] = audio_priority_find(a - 5, b);
    D_standin[1] = audio_priority_find(a, b);
}
