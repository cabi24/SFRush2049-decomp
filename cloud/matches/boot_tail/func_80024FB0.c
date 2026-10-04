/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct StreamState {
    unsigned char unknown_0000[4573];
    signed char state;
    signed char scale;
    unsigned char unknown_11DF[33];
    volatile unsigned char busy;
    unsigned char unknown_1201[39];
} StreamState;
extern unsigned char D_80038290;

void func_80024FB0(StreamState *stream)
{
    if (D_80038290 != 0) {
        stream->scale *= 2;
    }
}
