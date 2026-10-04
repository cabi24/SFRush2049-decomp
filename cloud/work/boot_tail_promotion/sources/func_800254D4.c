/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_800254D4.c: file-local type names StreamState suffixed _800254D4 so several bodies share one ROM TU; no other change. */
typedef struct StreamState_800254D4 {
    unsigned char unknown_0000[4573];
    signed char state;
    signed char scale;
    unsigned char unknown_11DF[33];
    volatile unsigned char busy;
    unsigned char value1;
    unsigned char value2;
    unsigned char value3;
    unsigned char unknown_1204[8];
    int handle;
    void *buffer;
    unsigned int buffer_count;
    unsigned char request_state;
    unsigned char unknown_1219[3];
    unsigned int processed;
    float field_1220;
    unsigned int token;
} StreamState_800254D4;
extern StreamState_800254D4 D_80056230[2];
extern unsigned int D_800586A0;
extern void (*D_8003801C)(void *);
extern int func_800250F0(void);
extern void func_80025120(int);
extern void func_80026348(StreamState_800254D4 *);

void func_800254D4(int selected)
{
    int token;

    switch (D_80056230[selected].busy) {
    case 1:
        D_80056230[selected].busy = 0;
        if (!(D_800586A0 & 1)) {
            D_8003801C(D_80056230[selected].buffer);
        }
        break;
    case 2:
        token = func_800250F0();
        func_80026348(&D_80056230[selected]);
        D_80056230[selected].busy = 3;
        D_80056230[selected].request_state = 4;
        func_80025120(token);
        break;
    }
}
