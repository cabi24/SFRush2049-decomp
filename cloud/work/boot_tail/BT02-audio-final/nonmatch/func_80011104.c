/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct AudioState {
    unsigned char active;
    unsigned char unknown01[0x5F];
    unsigned char bit_index;
    unsigned char changed;
    unsigned char unknown62[6];
} AudioState;
extern void *(*D_80038018)(unsigned int, unsigned int);
extern AudioState *D_80038294;
extern AudioState *D_80038298;
extern unsigned short D_8003829C;
extern unsigned char D_8003829E;
extern unsigned char D_80038290;
extern float D_8002D8B8;
extern float D_8002D8BC;
extern unsigned short D_800382CE;
extern void *D_800382D8;
extern void *D_800382DC;
extern unsigned short D_800382E0[2];
extern void *D_800382E4;
extern void func_80008590(void *, int);
extern void func_80007CA0(void *, int);
void func_80011104(unsigned short count, unsigned int frequency)
{
    unsigned short i;
    D_8003829C = count;
    D_80038298 = D_80038018(D_8003829C * sizeof(AudioState), 0);
    D_80038294 = D_80038298;
    D_8003829E = 0;
    for (i = 0; i < D_8003829C; i++) {
        D_80038298[i].active = 0;
        D_80038298[i].changed = 0;
        D_80038298[i].bit_index = 255;
    }
    if (D_80038290 == 0) {
        D_800382CE = (unsigned int) (((float)frequency / 50.0f / 192.0f) * D_8002D8B8) + 1;
    } else {
        D_800382CE = (unsigned int) (((float)frequency / 25.0f / 192.0f) * D_8002D8BC) + 1;
    }
    D_800382D8 = D_80038018(D_800382CE * 2576U, 0);
    D_800382DC = D_80038018(D_800382CE * 2576U, 0);
    D_800382E0[1] = 0;
    D_800382E0[0] = 0;
    D_800382E4 = D_80038018(664, 0);
    func_80008590(D_800382E4, 664);
    func_80007CA0(D_800382E4, 664);
}
