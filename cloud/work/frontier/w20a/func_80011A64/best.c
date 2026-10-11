/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct AudioState {
    unsigned char active;
    unsigned char held;
    unsigned char unknown02[2];
    unsigned short rate;
    unsigned short unknown06;
    double position;
    unsigned int current;
    unsigned int previous;
    unsigned char unknown18[8];
    unsigned short initial_count;
    unsigned short decay_count;
    float sustain;
    unsigned short release_count;
    unsigned char unknown2A[6];
    unsigned int length;
    unsigned int loop_start;
    unsigned int loop_length;
    unsigned int stop_loop;
    unsigned char unknown40[8];
    unsigned short count;
    unsigned short unknown4A;
    float value;
    unsigned int step;
    float scale;
    float saved_value;
    unsigned char state;
    unsigned char unknown5D[0xB];
} AudioState;
extern unsigned int D_8003828C;
void func_80011A64(AudioState *audio, unsigned short samples, unsigned char *active)
{
    unsigned int increment;
    float ratio;
    increment = ((samples * 32000U) / D_8003828C) << 11;
    if (audio->held == 0 && audio->state != 3) {
        audio->state = 3;
        audio->step = 0;
        audio->saved_value = audio->value;
        audio->scale = 0.0f;
        audio->count = audio->release_count;
    } else if ((audio->step >> 16) >= audio->count) {
        switch (audio->state) {
        case 0:
            audio->state = 1;
            audio->step = 0;
            audio->saved_value = 1.0f;
            audio->scale = audio->sustain;
            audio->count = audio->decay_count;
            break;
        case 1:
            audio->state = 2;
            audio->saved_value = audio->sustain;
            audio->scale = audio->sustain;
            audio->value = audio->sustain;
            return;
        case 3:
            *active = 0;
            audio->value = 0.0f;
            return;
        }
    }
    if (audio->count != 0) {
        ratio = audio->step / (audio->count * 65536.0f);
    } else {
        ratio = 1.0f;
    }
    audio->value = (audio->scale - audio->saved_value) * ratio + audio->saved_value;
    audio->step += increment;
}
