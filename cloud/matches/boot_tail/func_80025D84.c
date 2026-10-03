/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct StreamState {
    unsigned char unknown_0000[4573];
    signed char state;
    signed char scale;
    unsigned char unknown_11DF[33];
    volatile unsigned char busy;
    unsigned char unknown_1201[39];
} StreamState;
extern StreamState D_80056230[2];

void func_80025D84(void)
{
    int i;

    do {
        for (i = 0; i < 2; i++) {
            if (D_80056230[i].busy != 0) {
                break;
            }
        }
    } while (i != 2);
}
