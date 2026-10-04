/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct AudioConfiguration {
    unsigned short duration;
    unsigned short delays[8];
    unsigned char gains[8];
    unsigned char count;
    unsigned char feedback;
    unsigned char mode;
    unsigned char unknown1D;
    short filter[4];
} AudioConfiguration;
typedef struct AudioDelayState {
    void *buffer;
    unsigned int length;
    unsigned short count;
    unsigned short feedback;
    unsigned int delays[8];
    unsigned short gains[8];
    unsigned int unknown3C;
    short filter[4];
} AudioDelayState;
extern unsigned int D_8003828C;
extern void *D_800382F0;
extern AudioDelayState *D_800382E8;
extern unsigned int D_800382EC;
extern void *(*D_80038018)(unsigned int, unsigned int);
extern void bzero(void *, int);
extern void osWritebackDCache(void *, int);
extern void func_80014594(void);
extern void func_800145DC(void);
void func_80010E80(AudioConfiguration *configuration)
{
    AudioDelayState *state;
    unsigned int count;
    unsigned int i;
    if (D_800382F0 != 0) {
        count = configuration->duration * D_8003828C / 1000;
        count += 192U - count % 192U;
        state = D_80038018(72, 0);
        state->buffer = D_800382F0;
        bzero(state->buffer, count * 2);
        osWritebackDCache(state->buffer, count * 2);
        state->length = count;
        state->feedback = configuration->feedback << 8;
        state->count = configuration->count;
        for (i = 0; i < configuration->count; i++) {
            state->delays[i] = (configuration->delays[i] * D_8003828C / 1000 + 3) & 0xFFFC;
            state->gains[i] = configuration->gains[i] << 8;
        }
        if (configuration->mode == 1) {
            state->filter[0] = configuration->filter[0];
            state->filter[1] = configuration->filter[1];
            state->filter[2] = configuration->filter[2];
            state->filter[3] = configuration->filter[3];
        } else {
            state->filter[0] = 0;
            state->filter[1] = 0;
            state->filter[2] = 0;
            state->filter[3] = 32767;
        }
        osWritebackDCache(state, 72);
        func_80014594();
        D_800382EC = 0;
        D_800382E8 = state;
        func_800145DC();
    }
}
