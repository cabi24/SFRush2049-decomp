/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
typedef int (*SampleCallback)(short *, u32, short *, u32, u32);
typedef struct SampleBuffer {
    u8 mode;
    u8 unknown01[3];
    SampleCallback callback;
    short *buffer;
    u32 samples;
    u32 position;
    u32 context;
} SampleBuffer;
extern u8 D_8004FA18;
extern SampleBuffer D_8004FA50[];
extern u32 func_80014C18(int);
extern void func_80014C40(short *, u32);
void func_8001C3CC(void)
{
    int i;
    u32 position;
    for (i = 0; i < D_8004FA18; i++) {
        if (D_8004FA50[i].mode == 1) {
            position = func_80014C18(i);
            if (position != D_8004FA50[i].position) {
                if (position > D_8004FA50[i].position) {
                    if (D_8004FA50[i].callback(D_8004FA50[i].buffer + D_8004FA50[i].position,
                        position - D_8004FA50[i].position, 0, 0, D_8004FA50[i].context)) {
                        func_80014C40(D_8004FA50[i].buffer + D_8004FA50[i].position,
                            position - D_8004FA50[i].position);
                    }
                } else {
                    if (D_8004FA50[i].callback(D_8004FA50[i].buffer + D_8004FA50[i].position,
                        D_8004FA50[i].samples - D_8004FA50[i].position,
                        D_8004FA50[i].buffer, position, D_8004FA50[i].context)) {
                        func_80014C40(D_8004FA50[i].buffer + D_8004FA50[i].position,
                            D_8004FA50[i].samples - D_8004FA50[i].position);
                        func_80014C40(D_8004FA50[i].buffer, position);
                    }
                }
            }
            D_8004FA50[i].position = position;
        }
    }
}
