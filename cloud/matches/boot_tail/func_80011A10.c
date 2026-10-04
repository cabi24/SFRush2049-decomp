/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct AudioState {
    unsigned char unknown00[0x20];
    unsigned short initial_count;
    unsigned char unknown22[6];
    unsigned short release_count;
    unsigned char unknown2A[0x1E];
    unsigned short count;
    unsigned short unknown4A;
    float value;
    unsigned int step;
    float scale;
    float saved_value;
    unsigned char state;
} AudioState;
void func_80011A10(AudioState *audio)
{
    audio->state = 0;
    audio->step = 0;
    audio->saved_value = 0.0f;
    audio->value = 0.0f;
    audio->count = audio->initial_count;
    audio->scale = 1.0f;
}
