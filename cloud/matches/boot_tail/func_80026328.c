/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct StreamState {
    unsigned char unknown_0000[4573];
    signed char state;
    signed char scale;
    unsigned char unknown_11DF[33];
    volatile unsigned char busy;
    unsigned char unknown_1201[39];
} StreamState;

void func_80026328(StreamState *stream)
{
    if (stream->state == 2) {
        stream->state = 3;
    }
}
