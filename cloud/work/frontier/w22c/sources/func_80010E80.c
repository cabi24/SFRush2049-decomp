/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80010E80 (lib_11640, IDO 7.1): delay-line setup. w22c, traced with tools/trace71.
 * The retail colouring keeps the rounded length `count` in v1 across the three calls (ugen save/reload) because
 * 7.1's callee-saved cost is max(4, nblocks/4): the mode-1 filter copy written as a 4-trip loop gives the procedure
 * 20 blocks at globalcolor (16 with four statements), so the callee cost is 5 = the caller cost and v1 wins the tie.
 * One variable for the duration product and the rounded length (`count`), one for the byte size and both loop
 * indices (`i`); `count` declared first (its home is retail's sp+44). No devices, no unused locals.
 * The parameter is `void *` like the TU's caller prototype (lib_11640 func_80011074 area); the private view
 * AudioConfiguration_80010E80 is reached by casts (a typed local copy would add a frame slot). */
typedef struct AudioConfiguration_80010E80 {
    unsigned short duration;
    unsigned short delays[8];
    unsigned char gains[8];
    unsigned char count;
    unsigned char feedback;
    unsigned char mode;
    unsigned char unknown1D;
    short filter[4];
} AudioConfiguration_80010E80;
typedef struct AudioDelayState_80010E80 {
    void *buffer;
    unsigned int length;
    unsigned short count;
    unsigned short feedback;
    unsigned int delays[8];
    unsigned short gains[8];
    unsigned int unknown3C;
    short filter[4];
} AudioDelayState_80010E80;
extern unsigned int D_8003828C;
extern void *D_800382F0;
extern void *D_800382E8;
extern int D_800382EC;
extern void *(*D_80038018)(unsigned int, unsigned int);
extern void bzero(void *, int);
extern void osWritebackDCache(void *, int);
extern void func_80014594(void);
extern void func_800145DC(void);
void func_80010E80(void *configuration)
{
    unsigned int count;
    AudioDelayState_80010E80 *state;
    unsigned int i;
    if (D_800382F0 != 0) {
        count = ((AudioConfiguration_80010E80 *)configuration)->duration * D_8003828C / 1000;
        count = 192 - count % 192 + count;
        state = D_80038018(72, 0);
        state->buffer = D_800382F0;
        i = count * 2;
        bzero(state->buffer, i);
        osWritebackDCache(state->buffer, i);
        state->length = count;
        state->feedback = ((AudioConfiguration_80010E80 *)configuration)->feedback << 8;
        state->count = ((AudioConfiguration_80010E80 *)configuration)->count;
        for (i = 0; i < ((AudioConfiguration_80010E80 *)configuration)->count; i++) {
            state->delays[i] = (((AudioConfiguration_80010E80 *)configuration)->delays[i] * D_8003828C / 1000 + 3) & 0xFFFC;
            state->gains[i] = ((AudioConfiguration_80010E80 *)configuration)->gains[i] << 8;
        }
        if (((AudioConfiguration_80010E80 *)configuration)->mode == 1) {
            for (i = 0; i < 4; i++) {
                state->filter[i] = ((AudioConfiguration_80010E80 *)configuration)->filter[i];
            }
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
