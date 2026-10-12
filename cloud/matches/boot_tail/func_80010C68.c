/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct OSMesgQueue_s OSMesgQueue;
typedef struct OSThread_s OSThread;
extern unsigned char D_80038220;
extern unsigned char D_800381F8[];
extern void *D_80038210[];
extern unsigned int D_8003828C;
extern unsigned char *D_800381F0;
extern unsigned char D_80038040[];
extern void *(*D_80038018)(unsigned int, unsigned int);
extern void osCreateMesgQueue(OSMesgQueue *, void **, int);
extern void osSetEventMesgAlt(int, OSMesgQueue *, void *);
extern int osAiSetFrequency(unsigned int);
extern void osCreateThread(OSThread *, int, void (*)(void *), void *, void *, int);
extern void osStartThread(OSThread *);
extern void func_80010A40(void *);
void func_80010C68(unsigned int *frequency)
{
    D_80038220 = 1;
    osCreateMesgQueue((OSMesgQueue *) D_800381F8, D_80038210, 4);
    osSetEventMesgAlt(6, (OSMesgQueue *) D_800381F8, &D_80038220);
    D_8003828C = osAiSetFrequency(*frequency);
    *frequency = D_8003828C;
    D_800381F0 = D_80038018(1024, 128);
    osCreateThread((OSThread *) D_80038040, 0, func_80010A40, 0,
                   D_800381F0 + 1024, 122);
    osStartThread((OSThread *) D_80038040);
}
