/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct OSMesgQueue_s OSMesgQueue;
extern unsigned char D_80038290;
extern unsigned char D_80038291;
extern volatile unsigned char D_80038288;
extern unsigned short D_80038292;
extern unsigned int D_8003828C;
extern unsigned char *D_80038228[];
extern OSMesgQueue D_800381F8;
extern void *(*D_80038018)(unsigned int, unsigned int);
extern void bzero(void *, int);
extern void osWritebackDCache(void *, int);
extern int osAiSetNextBuffer(void *, unsigned int);
extern int osRecvMesg(OSMesgQueue *, void **, int);
void func_80010A40(void *argument)
{
    int i;
    int stop;
    unsigned char *message;
    if (D_80038290 != 0) {
        D_80038292 = 24;
    } else {
        D_80038292 = 16;
    }
    if (D_8003828C <= 22050) {
        D_80038292 >>= 1;
    }
    D_80038228[0] = D_80038018(D_80038292 * 768, 128);
    for (i = 1; i < D_80038292; i++) {
        D_80038228[i] = D_80038228[i - 1] + 768;
    }
    bzero(D_80038228[0], D_80038292 * 768);
    osWritebackDCache(D_80038228[0], D_80038292 * 768);
    D_80038288 = 0;
    osAiSetNextBuffer(D_80038228[0], 768);
    stop = 0;
    do {
        osRecvMesg(&D_800381F8, (void **) &message, 1);
        if (D_80038291 == 0) {
            switch (*message) {
            case 1:
                D_80038288 = (D_80038288 + 1) % D_80038292;
                osAiSetNextBuffer(D_80038228[D_80038288], 768);
                break;
            case 255:
                stop = 1;
                break;
            }
        }
    } while (stop == 0);
}
