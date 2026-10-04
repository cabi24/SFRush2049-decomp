/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80026328.c: file-local type names StreamState suffixed _80026328 so several bodies share one ROM TU; no other change. */
typedef struct StreamState_80026328 {
    unsigned char unknown_0000[4573];
    signed char state;
    signed char scale;
    unsigned char unknown_11DF[33];
    volatile unsigned char busy;
    unsigned char unknown_1201[39];
} StreamState_80026328;

void func_80026328(StreamState_80026328 *stream)
{
    if (stream->state == 2) {
        stream->state = 3;
    }
}
