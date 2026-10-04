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
extern double func_8001E688(double);
void func_80011D74(AudioState *audio, unsigned short samples)
{
    double increment;
    double loop_end;
    increment = (audio->rate * samples) * 0.000244140625;
    if (audio->loop_length != 0 && (audio->held != 0 || audio->stop_loop == 0)
        && func_8001E688(audio->position) <= audio->loop_start + audio->loop_length) {
        loop_end = audio->loop_start + audio->loop_length;
        if (audio->position + increment < loop_end) {
            audio->position += increment;
        } else {
            increment -= loop_end - audio->position;
            increment -= func_8001E688(increment / audio->loop_length) * audio->loop_length;
            audio->position = audio->loop_start + increment;
        }
    } else {
        if (audio->position + increment >= audio->length) {
            audio->position = audio->length - 1;
        } else {
            audio->position += increment;
        }
    }
}
